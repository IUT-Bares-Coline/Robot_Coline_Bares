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
        public MainWindow()
        {
            InitializeComponent();
        }

        private void SendMessage()
        {
        //textBoxEmission.Text = "bob";
        textBoxReception.Text = "\nReçu : " + textBoxEmission.Text;
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
