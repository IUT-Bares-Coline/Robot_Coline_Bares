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

            serialPort1 = new ReliableSerialPort("COM4", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
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
            int i;                       
            byte[] byteList = new byte[20];
            for (i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList, 0, byteList.Length);
        }
    }

    /*
    byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload )
    {
        int checksum = 0;
        checksum ^= 0xFE ;
        checksum ^= (byte)(msgFunction>>8);
        checksum ^= (byte)(mmsgPayloadLengt >> 0);
    }
    */
}
