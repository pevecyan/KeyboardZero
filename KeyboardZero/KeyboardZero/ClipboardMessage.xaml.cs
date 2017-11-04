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
using System.Windows.Shapes;

namespace KeyboardZero
{
    /// <summary>
    /// Interaction logic for ClipboardMessage.xaml
    /// </summary>
    public partial class ClipboardMessage : Window
    {
        public ClipboardMessage()
        {
            InitializeComponent();
        }

        public ClipboardMessage(string message)
        {
            InitializeComponent();

            this.MessageTextBlock.Text = message;

            this.Hide_BeginStoryboard.Storyboard.Completed += delegate
            {
                this.Close();
            };
        }
    }
}
