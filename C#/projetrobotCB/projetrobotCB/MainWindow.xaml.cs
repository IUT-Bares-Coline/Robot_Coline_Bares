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

namespace projetrobotCB
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM8", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();
        }
        
        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            textBoxReception.Text += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length) ;
        }

        private void SendMessage()
        {
        serialPort1.WriteLine(textBoxEmission.Text); 
        //textBoxReception.Text = textBoxReception.Text + "\nReçu : " + textBoxEmission.Text;
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
    }
}
