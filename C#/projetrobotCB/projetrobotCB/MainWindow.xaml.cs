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

namespace projetrobotCB
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Robot robot = new Robot();
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage ;

        public MainWindow()
        {
            InitializeComponent();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

            serialPort1 = new ReliableSerialPort("COM5", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();
        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            //throw new NotImplementedException();
            while(robot.receivedBytes.Count>0)
            {
                byte b = robot.receivedBytes.Dequeue();
                textBoxReception.Text += " 0x" + b.ToString("X2");
            }
            //if (robot.receivedText != null)
            //{
            //    textBoxReception.Text = textBoxReception.Text + "Reçu : " + robot.receivedText;
            //    robot.receivedText = null;
            //}
            
        }

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            foreach(byte b in e.Data)
            {
                robot.receivedBytes.Enqueue(b);
            }
            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);            
            //textBoxReception.Text += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length) ;
        }

        private void SendMessage()
        {
            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxEmission.Text = "";
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
         if(buttonEnvoyer.Background == Brushes.RoyalBlue) 
            {
                buttonEnvoyer.Background = Brushes.Beige;
            }
         else
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }
            
            SendMessage();
        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
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
            /*
            int i;                       
            byte[] byteList = new byte[20];
            for (i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList, 0, byteList.Length);
            */

            string s = "Bonjour";
            byte[] array = Encoding.ASCII.GetBytes(s);
            UartEncodeAndSendMessage(0x0080, (UInt16)array.Length, array);
            
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

            /*
            int checksum = 0;
            checksum ^= 0xFE ;
            checksum ^= (byte)(msgFunction>>8);
            checksum ^= (byte)(msgPayloadLength >> 8);

            for (int i= 0; i< msgPayloadLength; i++)
            {
                //checksum ^= msgPayload[i] >> 8;
            }
            */

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
            Waiting ,
            FunctionMSB ,
            FunctionLSB ,
            PayloadLengthMSB ,
            PayloadLengthLSB ,
            Payload ,
            CheckSum
        }
        
        StateReception rcvState = StateReception.Waiting ;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload ;
        int msgDecodedPayloadIndex = 0;
        
        private void DecodeMessage(byte c)
        {
            switch (rcvState )
            {
                case StateReception.Waiting:

                    if (c == 0xFE) rcvState = StateReception.FunctionMSB;
                    break;

                case StateReception.FunctionMSB:

                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:

                    msgDecodedFunction += c ;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;

                case StateReception.PayloadLengthMSB:

                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    
                    msgDecodedPayloadLength += c;
                    rcvState = StateReception.Payload;
                    break;

                case StateReception.Payload:

                    msgDecodedPayload = c ;            //à verifier
                    for(int i=0; i< msgDecodedPayloadLength/8; i++)
                    {
                        msgDecodedPayload += c << 8;
                    }
                    rcvState = StateReception.CheckSum;
                    break;

                case StateReception.CheckSum:

                    byte calculatedChecksum = CalculateChecksum((ushort)msgDecodedFunction, (ushort)msgDecodedPayloadLength, msgDecodedPayload);
                    byte receivedChecksum = c;

                    if (calculatedChecksum == receivedChecksum )
                        {
                        // Success , on a un message valide
                        }
                    . . .
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }


    }





}















