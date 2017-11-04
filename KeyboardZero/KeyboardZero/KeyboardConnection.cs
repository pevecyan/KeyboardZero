using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace KeyboardZero
{

    class KeyboardConnection
    {
        public bool IsConnected { get; set; }
        public SerialPort ActiveSerialPort { get; set;}
        public string DeviceName { get; set; }
        public Point Size { get; set; }

        public event Action<string> KeyPressed;

        public void FindKeyboard()
        {
            var serialNames = SerialPort.GetPortNames();
            foreach(var serialName in serialNames)
            {
                SerialPort serialPort = new SerialPort(serialName, 9600); 
                try
                {
                    serialPort.RtsEnable = true;
                    serialPort.DtrEnable = true;
                    serialPort.Open();
                    serialPort.Write("ACK");
                    Thread.Sleep(2000);
                    var data = serialPort.ReadExisting();
                    Console.WriteLine(data);
                    if(data != "" && data.IndexOf(':') > - 1 && data.Split(':').Length >= 3 )
                    {
                        var keyboardData = data.Split(':');
                        this.DeviceName = keyboardData[0];
                        this.Size = new Point(int.Parse(keyboardData[1]), int.Parse(keyboardData[2]));
                        serialPort.Write("OK");
                        this.ActiveSerialPort = serialPort;
                        this.IsConnected = true;
                        this.ActiveSerialPort.DataReceived += CommandReceived;
                        break;
                    }
                    else
                    {
                        serialPort.Close();
                        continue;
                    }
                }
                catch (Exception ex) {
                    if(serialPort.IsOpen) serialPort.Close();
                    Console.WriteLine(ex.Message);
                }
            }
        }

        private void CommandReceived(object sender, SerialDataReceivedEventArgs e)
        {

            var key = (sender as SerialPort).ReadExisting();
            KeyPressed?.Invoke(key);
        }
    }
}
