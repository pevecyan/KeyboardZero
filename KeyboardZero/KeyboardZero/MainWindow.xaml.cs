using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WindowsInput;
using WindowsInput.Native;

namespace KeyboardZero
{

    #region transparent blur
    internal enum AccentState
    {
        ACCENT_DISABLED = 0,
        ACCENT_ENABLE_GRADIENT = 1,
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        ACCENT_ENABLE_BLURBEHIND = 3,
        ACCENT_INVALID_STATE = 4
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct AccentPolicy
    {
        public AccentState AccentState;
        public int AccentFlags;
        public int GradientColor;
        public int AnimationId;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct WindowCompositionAttributeData
    {
        public WindowCompositionAttribute Attribute;
        public IntPtr Data;
        public int SizeOfData;
    }

    internal enum WindowCompositionAttribute
    {
        // ...
        WCA_ACCENT_POLICY = 19
        // ...
    }
    #endregion
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("user32.dll")]
        internal static extern int SetWindowCompositionAttribute(IntPtr hwnd, ref WindowCompositionAttributeData data);

        public Point startMovingPosition;
        public bool movingWindow = false;

        KeyboardConnection keyboardConnection;

        
        public MainWindow()
        {
            InitializeComponent();

            DiscoverKeyboard();
           
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);

            var windowClipboardManager = new ClipboardManager(this);
            windowClipboardManager.ClipboardChanged += ClipboardChanged;
        }





        public void DiscoverKeyboard()
        {
            keyboardConnection = new KeyboardConnection();
            BackgroundWorker bw = new BackgroundWorker();

            bw.DoWork += (sender, args) =>
              {
                 // while (!keyboardConnection.IsConnected)
                  {
                      //Thread.Sleep(15000);
                      keyboardConnection.FindKeyboard();
                      if (keyboardConnection.IsConnected)
                      {
                          Dispatcher.Invoke(delegate(){ ShowKeyboard(); });
                      }
                  }
              };
            bw.RunWorkerAsync();
        }
        

        private void ShowKeyboard()
        {

            keyboardConnection.KeyPressed += (key) =>
            {
                //{ '0','1','2','3','4','5'},
                //{ '6','7','8','9','a','b'},
                //{ 'c','d','e','f','g','h'},
                //{ 'i','j','k','l','m','n'}

                Console.WriteLine(key);
                KeyboardSimulator keyboardSimulator = new KeyboardSimulator(new InputSimulator());
                switch (key)
                {
                    case "0":
                        Dispatcher.Invoke(() => {
                            internalClipboardChange = 2;
                            Clipboard.SetText(clipboardStack[0]);
                            showClipboardMessage(clipboardStack[0]);
                        });
                        break;
                    case "1":
                        Dispatcher.Invoke(() => {
                            internalClipboardChange = 2;
                            Clipboard.SetText(clipboardStack[1]);
                            showClipboardMessage(clipboardStack[1]);
                        });
                        break;
                    case "2":
                        Dispatcher.Invoke(() => {
                            internalClipboardChange = 2;
                            Clipboard.SetText(clipboardStack[2]);
                            showClipboardMessage(clipboardStack[2]);
                        });
                        break;
                    case "3":
                        Dispatcher.Invoke(() => {
                            internalClipboardChange = 2;
                            Clipboard.SetText(clipboardStack[3]);
                            showClipboardMessage(clipboardStack[3]);
                        });
                        break;
                    case "4":
                        keyboardSimulator.KeyPress(VirtualKeyCode.MEDIA_PREV_TRACK);
                        break;
                    case "5":
                        keyboardSimulator.KeyPress(VirtualKeyCode.MEDIA_NEXT_TRACK);
                        break;
                    case "d":
                        System.Diagnostics.Process.Start(@"C:\Users\pevec\AppData\Local\hyper\Hyper.exe");
                        break;
                    case "i":
                        keyboardSimulator.ModifiedKeyStroke(new[] { VirtualKeyCode.LCONTROL, VirtualKeyCode.LMENU }, VirtualKeyCode.VK_1);
                        break;
                    case "j":
                        keyboardSimulator.ModifiedKeyStroke(new[] { VirtualKeyCode.LCONTROL, VirtualKeyCode.LMENU }, VirtualKeyCode.VK_2);
                        break;
                    case "k":
                        keyboardSimulator.ModifiedKeyStroke(new[] { VirtualKeyCode.LCONTROL, VirtualKeyCode.LMENU }, VirtualKeyCode.VK_3);
                        break;
                    case "l":
                        keyboardSimulator.ModifiedKeyStroke(new[] { VirtualKeyCode.LCONTROL, VirtualKeyCode.LMENU }, VirtualKeyCode.VK_4);
                        break;
                    case "m":
                        keyboardSimulator.ModifiedKeyStroke(new[] { VirtualKeyCode.LCONTROL, VirtualKeyCode.LMENU }, VirtualKeyCode.VK_5);
                        break;
                    case "n":
                        keyboardSimulator.ModifiedKeyStroke(new[] { VirtualKeyCode.LCONTROL, VirtualKeyCode.LMENU }, VirtualKeyCode.VK_6);
                        break;

                }
                //

            };

            DeviceName.Text = keyboardConnection.DeviceName;
            for(int y = 0; y < keyboardConnection.Size.Y; y++)
            {
                var rowPanel = new StackPanel() { Orientation = Orientation.Horizontal, HorizontalAlignment = HorizontalAlignment.Center };
                for (int x = 0; x < keyboardConnection.Size.X; x++)
                {
                    var key = new Grid() { Height = 50, Width = 50, Background = new SolidColorBrush(Color.FromArgb(100, 0, 0, 0)), Margin = new Thickness(5) };
                    rowPanel.Children.Add(key);
                }
                LayoutGrid.Children.Add(rowPanel);
            }
            DeviceGrid.Visibility = Visibility.Visible;
            StatusTitle.Visibility = Visibility.Collapsed;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            EnableBlur();
        }

        internal void EnableBlur()
        {
            var windowHelper = new WindowInteropHelper(this);

            var accent = new AccentPolicy();
            accent.AccentState = AccentState.ACCENT_ENABLE_BLURBEHIND;

            var accentStructSize = Marshal.SizeOf(accent);

            var accentPtr = Marshal.AllocHGlobal(accentStructSize);
            Marshal.StructureToPtr(accent, accentPtr, false);

            var data = new WindowCompositionAttributeData();
            data.Attribute = WindowCompositionAttribute.WCA_ACCENT_POLICY;
            data.SizeOfData = accentStructSize;
            data.Data = accentPtr;

            SetWindowCompositionAttribute(windowHelper.Handle, ref data);

            Marshal.FreeHGlobal(accentPtr);
        }

        private void Topbar_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == System.Windows.Input.MouseButton.Left && e.MouseDevice.DirectlyOver == sender || e.MouseDevice.DirectlyOver == TitleBlock)
                this.DragMove();
        }

        private void CloseButton_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            this.Close();
        }

        //Clipboard
        string [] clipboardStack = {"","","","" };
        int internalClipboardChange = 0;
        private void ClipboardChanged(object sender, EventArgs e)
        {
            // Handle your clipboard update here, debug logging example:
            if (Clipboard.ContainsText() && internalClipboardChange == 0)
            {
                clipboardStack[3] = clipboardStack[2];
                clipboardStack[2] = clipboardStack[1];
                clipboardStack[1] = clipboardStack[0];
                clipboardStack[0] = Clipboard.GetText();
                Console.WriteLine(clipboardStack[0]);
                keyboardConnection.SendMessage(KeyboardConnection.SendTypes.ClipboardPush, clipboardStack[0], 0);
            }
            else if(internalClipboardChange > 0)
            {
                internalClipboardChange--;
            }
            
        }

        private void showClipboardMessage(string text)
        {
            return;
            var message = new ClipboardMessage(text);
            message.Top = 10;
            message.Left = 10;
            message.Show();
        }
    }
}
