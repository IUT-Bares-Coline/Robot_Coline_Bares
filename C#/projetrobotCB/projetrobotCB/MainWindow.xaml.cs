using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

using System.Windows.Forms;

//Dans Form1.cs ?? :
using MouseKeyboardActivityMonitor.WinApi;
using MouseKeyboardActivityMonitor;

using Utilities;

namespace projetrobotCB
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    /// 

    public enum MsgFunctions
    {
        TextTransmission = 0x0080,
        LEDSetUp = 0x0020,
        IRDistance = 0x0030,
        SpeedSetUp = 0x0040,
        ChangeState = 0x0050,
        SetRobotState = 0x0051,
        SetRobotManualControl = 0x0052,
        DataPosition = 0x0061
    }

    public enum StateRobot
    {
        STATE_ATTENTE = 0,
        STATE_ATTENTE_EN_COURS = 1,
        STATE_AVANCE = 2,
        STATE_AVANCE_EN_COURS = 3,
        STATE_TOURNE_GAUCHE = 4,
        STATE_TOURNE_GAUCHE_EN_COURS = 5,
        STATE_TOURNE_DROITE = 6,
        STATE_TOURNE_DROITE_EN_COURS = 7,
        STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
        STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
        STATE_TOURNE_SUR_PLACE_DROITE = 10 ,
        STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11 ,
        STATE_ARRET = 12 ,
        STATE_ARRET_EN_COURS = 13 ,
        STATE_RECULE = 14 ,
        STATE_RECULE_EN_COURS = 15
    }


    public partial class MainWindow : Window
    {
        Robot robot = new Robot();
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;

        private readonly KeyboardHookListener m_KeyboardHookManager;

        public MainWindow()
        {
            InitializeComponent();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

            serialPort1 = new ReliableSerialPort("COM8", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            //à comprendre et à commenter (pour ne pas oublier) :

            m_KeyboardHookManager = new KeyboardHookListener(new GlobalHooker());
            m_KeyboardHookManager.Enabled = true;
            m_KeyboardHookManager.KeyDown += HookManager_KeyDown;
        }

        bool autoControlActivated = true;

        private void HookManager_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if(autoControlActivated == false )
            { 

                switch (e.KeyCode)
                {
                    case Keys.Left :
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] {( byte ) StateRobot.STATE_TOURNE_SUR_PLACE_GAUCHE }) ;
                        break;

                    case Keys.Right :
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] {( byte ) StateRobot.STATE_TOURNE_SUR_PLACE_DROITE });
                        break;
                    
                    case Keys.Up:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] {( byte ) StateRobot.STATE_AVANCE });
                        break;
                    
                    case Keys.PageDown:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] { ( byte ) StateRobot.STATE_ARRET });  
                        break;
                    
                    case Keys.Down :
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] { ( byte ) StateRobot.STATE_RECULE });
                        break;
                }
            }
        }



    private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            //throw new NotImplementedException();
            while (robot.MessageQueue.Count > 0)
            {
                Message m;
                if (robot.MessageQueue.TryDequeue(out m))
                {
                    ProcessDecodedMessage(m.Function, m.PayloadLength, m.Payload);
                    //foreach (var b in m.Payload)
                    //{
                    //    textBoxReception.Text += " 0x" + b.ToString("X2");
                    //}
                    //textBoxReception.Text += "\n";
                }

            }
        }

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e) //FIFO
        {
            foreach (byte b in e.Data)
            {
                DecodeMessage(b);
            }



        }

        private void SendMessage()
        {
            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxEmission.Text = "";
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (buttonEnvoyer.Background == Brushes.RoyalBlue)
            {
                buttonEnvoyer.Background = Brushes.Beige;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }

            SendMessage();

        }

        private void textBoxEmission_KeyUp(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }

        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            textBoxReception.Text = null;
        }


        private void ButtonTest_Click(object sender, RoutedEventArgs e)
        {
            string s = "Bonjour";
            byte[] array = Encoding.ASCII.GetBytes(s);
            UartEncodeAndSendMessage((ushort)MsgFunctions.TextTransmission, (UInt16)array.Length, array);
            UartEncodeAndSendMessage((ushort)MsgFunctions.LEDSetUp, 2, new byte[] { 1, 1 });
            UartEncodeAndSendMessage((ushort)MsgFunctions.IRDistance, 3, new byte[] { 10, 20, 50 });
            UartEncodeAndSendMessage((ushort)MsgFunctions.SpeedSetUp, 2, new byte[] { 50, 20 });

            
        }

        private byte CalculateChecksum(ushort msgFunction, ushort msgPayloadLength, byte[] msgPayload)
        {

            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);

            foreach (byte b in msgPayload)
            {
                checksum ^= b;
            }
            return checksum;
        }

        public void UartEncodeAndSendMessage(ushort msgFunction, ushort msgPayloadLength, byte[] msgPayload)
        {
            byte[] message = new byte[6 + msgPayloadLength];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8);
            message[pos++] = (byte)(msgFunction >> 0);
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];
            }



            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

            serialPort1.Write(message, 0, pos);
        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        UInt16 msgDecodedFunction = 0;
        UInt16 msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:

                    if (c == 0xFE) rcvState = StateReception.FunctionMSB;
                    break;

                case StateReception.FunctionMSB:

                    msgDecodedFunction = (UInt16)(c << 8);
                    rcvState = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:

                    msgDecodedFunction += c;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;

                case StateReception.PayloadLengthMSB:

                    msgDecodedPayloadLength = (UInt16)(c << 8);
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:

                    msgDecodedPayloadLength += c;
                    if (msgDecodedPayloadLength == 0)
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    else if (msgDecodedPayloadLength < 1024)
                    {
                        rcvState = StateReception.Payload;
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        msgDecodedPayloadIndex = 0;
                    }
                    else
                    {
                        rcvState = StateReception.Waiting;
                    }
                    break;

                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                    break;

                case StateReception.CheckSum:

                    byte calculatedChecksum = CalculateChecksum((ushort)msgDecodedFunction, (ushort)msgDecodedPayloadLength, msgDecodedPayload);
                    byte receivedChecksum = c;

                    if (calculatedChecksum == receivedChecksum)
                    {
                        // Success , on a un message valide
                        robot.MessageQueue.Enqueue(new Message(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload));
                    }
                    else
                    {
                        Console.WriteLine("Erreur de décodage de message");
                    }
                    rcvState = StateReception.Waiting;

                    break;

                default:
                    rcvState = StateReception.Waiting;
                    break;
            }

        }

        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch ((MsgFunctions)msgFunction)
            {
                case MsgFunctions.TextTransmission:
                    textBoxReception.Text += Encoding.UTF8.GetString(msgPayload, 0, msgPayload.Length)+"\n";
                    break;

                case MsgFunctions.SetRobotManualControl:
                    textBoxReception.Text += Encoding.UTF8.GetString(msgPayload, 0, msgPayload.Length) + "\n";
                    break;

                case MsgFunctions.LEDSetUp:
                    int numLed = msgPayload[0];
                    int ledState = msgPayload[1];
                    switch (numLed)
                    {
                        case 1:
                            if (ledState == 1)
                            {
                                led1.IsChecked = true;
                            }
                            else led1.IsChecked = false;
                            break;
                        case 2:
                            if (ledState == 1)
                            {
                                led2.IsChecked = true;
                            }
                            else led2.IsChecked = false;
                            break;
                        case 3:
                            if (ledState == 1)
                            {
                                led3.IsChecked = true;
                            }
                            else led3.IsChecked = false;
                            break;
                    }
                    break;

                case MsgFunctions.IRDistance:
                    int gauche = msgPayload[0];
                    labelTelemetreGauche.Content = "Telemètre gauche : " + gauche.ToString() + " cm";
                    int centre = msgPayload[1];
                    labelTelemetreCentre.Content = "Telemètre centre : " + centre.ToString() + " cm";
                    int droite = msgPayload[2];
                    labelTelemetreDroit.Content = "Telemètre droit : " + droite.ToString() + " cm";
                    break;

                case MsgFunctions.SpeedSetUp:
                    int moteurG = msgPayload[0];
                    labelVitesseG.Content = "Vitesse moteur gauche : " + moteurG.ToString() + " ";
                    int moteurD = msgPayload[1];
                    labelVitesseD.Content = "Vitesse moteur droit : " + moteurD.ToString() + " ";
                    break;

                case MsgFunctions.ChangeState:
                    int instant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16) 
                    + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
                    textBoxReception.Text += " \nRobot State : " +
                    ((StateRobot)(msgPayload[0])).ToString() +" − " + instant.ToString() + " ms";
                    break;

                case MsgFunctions.DataPosition:
                    byte[] tab = msgPayload.GetRange(4, 4); //4 de gauche = offset
                    robot.positionXOdo += tab.GetFloat();
                    textBoxReception.Text += "\nposition en x : " + robot.positionXOdo.ToString() + "\n";

                    break;
            }
        }

        private void Manuel_Click(object sender, RoutedEventArgs e)
        {
            string num = "0";
            byte[] numarray = Encoding.ASCII.GetBytes(num);
            UartEncodeAndSendMessage((ushort)MsgFunctions.SetRobotManualControl, 1, numarray);
            autoControlActivated = false;

        }

        private void automatique_Click(object sender, RoutedEventArgs e)
        {
            string num = "1";
            byte[] numarray = Encoding.ASCII.GetBytes(num);
            UartEncodeAndSendMessage((ushort)MsgFunctions.SetRobotManualControl, 1, numarray);
            autoControlActivated = true;
        }


        // pt d'arret : f5 = pt d'arret suivant - f10 = pas a pas - f9 = mettre un pt d'arret
    }
}















