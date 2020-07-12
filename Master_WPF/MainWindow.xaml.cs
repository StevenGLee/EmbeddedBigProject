using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
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

namespace Master_WPF
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort serial;
        TcpClient client;
        int length;
        byte[] buffer;
        WriteableBitmap CameraData;
        int ConnectionState;
        string AdcString;
        int AdcData;
        SequenceData AdcDatas, Data2Show;

        public MainWindow()
        {
            InitializeComponent();
            SerialPortComboBox.ItemsSource = SerialPort.GetPortNames();
            SerialPortComboBox.SelectedIndex = 0;
            buffer = new byte[1024 * 1024];
            ConnectionState = 0;
            AdcDatas = new SequenceData(20, 10000, 0);
        }

        private void StartTrans_Click(object sender, RoutedEventArgs e)
        {
            if (ConnectionState == 0)
            {
                if (EthernetRadioButton.IsChecked == true)
                {
                    Regex re = new Regex(@"^((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})(\.((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})){3}$");
                    if (!re.IsMatch(IPTextBox.Text))
                    {
                        MessageBox.Show("下位机地址不是有效的IPv4地址！");
                        return;
                    }
                    client = new TcpClient();
                    client.Connect(IPAddress.Parse(IPTextBox.Text), 10492);
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 使用Ethernet连接下位机。\n";
                    client.Client.BeginReceive(buffer, 0, 1024 * 1024, SocketFlags.None, OnRecieve, null);
                }
                if (UartRadioButton.IsChecked == true)
                {
                    serial = new SerialPort(SerialPortComboBox.Text);
                    serial.Open();
                    serial.BaseStream.BeginRead(buffer, 0, 1024 * 1024, OnRecieve, null);
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 使用UART连接下位机。\n";
                }
                StartTrans.Content = "断开连接";
                ConnectionState = 1;
            }
            else
            {
                if (EthernetRadioButton.IsChecked == true)
                {
                    client.Client.Send(Encoding.ASCII.GetBytes("exit"));
                    client.Close();
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 断开与下位机的Ethernet连接。\n";

                }
                if (UartRadioButton.IsChecked == true)
                {
                    serial.Close();
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 断开与下位机的UART连接。\n";
                }
                StartTrans.Content = "连接下位机";
                ConnectionState = 0;
            }
        }

        private void OnRecieve(IAsyncResult ar)
        {
            //收到TCP消息或uart消息
            if (EthernetRadioButton.IsChecked == true)
                length = client.Client.EndReceive(ar);
            else if (UartRadioButton.IsChecked == true)
                length = serial.BaseStream.EndRead(ar);
            else
                throw new Exception("不知道该从哪里read了！");
            if (CustomizedRadioButton.IsChecked == true)
            {
                switch (buffer[0])
                {
                    case 1://ADC传来的值
                        AdcString = Encoding.ASCII.GetString(buffer.Skip(1).ToArray());
                        AdcData = Convert.ToInt32(AdcString);
                        ADCDataTextbox.Text = "ADC值：" + AdcString;
                        AdcDataBar.Value = AdcData;
                        AdcDatas.AddSequenceData(AdcData);
                        AdcGraph.HotspotDatas = AdcDatas;
                        LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到了ADC传来的数据：" +AdcString +"。\n";
                        break;
                    case 2://Camera数据
                        if (length == 640 * 480 * 3 + 1)//接受不完整，便丢弃
                        {
                            CameraData = new WriteableBitmap(640, 480, 400, 400, PixelFormats.Rgb24, null);
                            //unsafe
                            {
                                CameraData.Lock();
                                Marshal.Copy(buffer.Skip(1).ToArray(), 0, CameraData.BackBuffer, 640 * 480 * 3); //请注意_wbBitmap的数据格式以及buffer大小，以免溢出和显示异常
                                CameraData.AddDirtyRect(new Int32Rect(0, 0, 640, 480));
                                CameraData.Unlock();
                            }
                            CameraImage.Source = CameraData;
                            LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到相机数据，已显示。\n";
                        }
                        else
                            LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到不完整相机数据，已丢弃。\n";
                        break;
                    default:
                        LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到了不认识的数据，已丢弃。\n";
                        break;
                }
            }
            if (EthernetRadioButton.IsChecked == true)
                client.Client.BeginReceive(buffer, 0, 1024 * 1024, SocketFlags.None, OnRecieve, client);
            else if (UartRadioButton.IsChecked == true)
                serial.BaseStream.BeginRead(buffer, 0, 1024 * 1024, OnRecieve, serial);

        }

        private void SaveCameraImage_Click(object sender, RoutedEventArgs e)
        {
            //CameraData.
            SaveCameraImg(CameraData);
        }

        /// <summary>
        /// C# WPF 保存WriteableBitmap图像
        /// </summary>
        /// <param name="wtbBmp"></param>
        /// 
        void SaveCameraImg(WriteableBitmap wtbBmp)
        {
            if (wtbBmp == null)
            {
                return;
            }
            try
            {
                RenderTargetBitmap rtbitmap = new RenderTargetBitmap(wtbBmp.PixelWidth, wtbBmp.PixelHeight, wtbBmp.DpiX, wtbBmp.DpiY, PixelFormats.Default);
                DrawingVisual drawingVisual = new DrawingVisual();
                using (var dc = drawingVisual.RenderOpen())
                {
                    dc.DrawImage(wtbBmp, new Rect(0, 0, wtbBmp.Width, wtbBmp.Height));
                }
                rtbitmap.Render(drawingVisual);
                JpegBitmapEncoder bitmapEncoder = new JpegBitmapEncoder();
                bitmapEncoder.Frames.Add(BitmapFrame.Create(rtbitmap));
                string strDir = @"./CameraImg/";
                string strpath = strDir + DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss") + ".jpg";
                if (!Directory.Exists(strDir))
                {
                    Directory.CreateDirectory(strDir);
                }
                if (!File.Exists(strpath))
                {
                    bitmapEncoder.Save(File.OpenWrite(strpath));
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }

        private void SaveADCGraph_Click(object sender, RoutedEventArgs e)
        {
            string strDir = @".\AdcGraph\";
            string strpath = strDir + DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss") + ".hst";
            if (!Directory.Exists(strDir))
            {
                Directory.CreateDirectory(strDir);
            }

            AdcDatas.SaveTo(strpath);
        }

        private void UartRadioButton_Click(object sender, RoutedEventArgs e)
        {

        }

        private void EthernetRadioButton_Click(object sender, RoutedEventArgs e)
        {

        }

        private void LoadADCGraph_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "HST Binary File(*.hst)|*.hst";
            openFileDialog.DefaultExt = ".hst"; // Default file extension
            string strDir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + @"\AdcGraph\";

            if (!Directory.Exists(strDir))
            {
                Directory.CreateDirectory(strDir);
            }

            openFileDialog.InitialDirectory = strDir;
            if (openFileDialog.ShowDialog() == true)//注意，此处一定要手动引入System.Window.Forms空间，否则你如果使用默认的DialogResult会发现没有OK属性
            {
                Data2Show = new SequenceData(0, 0, 0);
                Data2Show.ReadFrom(openFileDialog.FileName);
                ShowAdcData showAdcData = new ShowAdcData();
                showAdcData.AdcData = Data2Show;
                showAdcData.Show();
            }
        }
    }
}
