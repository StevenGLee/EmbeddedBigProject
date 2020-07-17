using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
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
        int ConnectionState;
        string AdcString;
        int AdcData;
        SequenceData AdcDatas, Data2Show;
        FileStream fs;
        int cnt;
        AdcDataset adcDataset;
        enum LowerProtocol
        {
            UART, Ethernet
        }
        LowerProtocol lowerProtocol;
        enum UpperProtocol
        {
            Customized, Modbus, MQTT
        }
        UpperProtocol upperProtocol;
        public MainWindow()
        {
            InitializeComponent();
            SerialPortComboBox.ItemsSource = SerialPort.GetPortNames();
            SerialPortComboBox.SelectedIndex = 0;
            buffer = new byte[1024 * 1024];
            ConnectionState = 0;
            AdcDatas = new SequenceData(20, 10000, 0);
            //ConsoleManager.Show();
            string path = @"./tmp/";
            string filename = path + "tmp.jpg";
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }

            fs = new FileStream(filename, FileMode.Create, FileAccess.ReadWrite, FileShare.ReadWrite);
            adcDataset = new AdcDataset();
            
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
                    try
                    {
                        client.Connect(IPAddress.Parse(IPTextBox.Text), 3230);
                    }
                    catch(Exception exception)
                    {
                        MessageBox.Show(exception.Message);
                        return;
                    }
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 使用Ethernet连接下位机。\n";
                    
                    client.Client.BeginReceive(buffer, 0, 1024 * 1024, SocketFlags.None, OnRecieve, null);
                    client.Client.Send(new byte[] { 123 });
                    lowerProtocol = LowerProtocol.Ethernet;
                }
                if (UartRadioButton.IsChecked == true)
                {
                    serial = new SerialPort(SerialPortComboBox.Text);
                    serial.BaudRate = 115200;
                    try
                    {
                        serial.Open();
                    }
                    catch (Exception exception)
                    {
                        MessageBox.Show(exception.Message);
                        return;
                    }
                    serial.BaseStream.BeginRead(buffer, 0, 1024 * 1024, OnRecieve, null);
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 使用UART连接下位机。\n";
                    //serial.DataReceived += OnDataRecieved;
                    lowerProtocol = LowerProtocol.UART;
                }
                StartTrans.Content = "断开连接";
                ConnectionState = 1;
                upperProtocol = UpperProtocol.Customized;//暂时这么设置上
                buffer = new byte[1024 * 1024];
                CameraDynamicRadioButton.IsEnabled = true;
                CameraStaticRadioButton.IsEnabled = true;
                CameraStopRadioButton.IsEnabled = true;
                CameraDynamicDetectionRadioButton.IsEnabled = false;//没开发出来
                AdcStopRadioButton.IsEnabled = true;
                AdcStaticRadioButton.IsEnabled = true;
                AdcDynamicRadioButton.IsEnabled = true;
            }
            else
            {
                if (lowerProtocol == LowerProtocol.Ethernet)
                {
                    //client.Client.EndReceive(null);
                    client.Client.Send(new byte[] { 255,(byte)'e', (byte)'x', (byte)'i', (byte)'t' });
                    client.Close();
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 断开与下位机的Ethernet连接。\n";

                }
                if (lowerProtocol == LowerProtocol.UART)
                {
                    //serial.BaseStream.EndRead(new AsyncResult( ));
                    serial.Close();
                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 断开与下位机的UART连接。\n";
                }
                StartTrans.Content = "连接下位机";
                ConnectionState = 0;
                CameraDynamicRadioButton.IsEnabled = false;
                CameraStaticRadioButton.IsEnabled = false;
                CameraStopRadioButton.IsEnabled = false;
                CameraDynamicDetectionRadioButton.IsEnabled = false;//没开发出来
                AdcStopRadioButton.IsEnabled = false;
                AdcStaticRadioButton.IsEnabled = false;
                AdcDynamicRadioButton.IsEnabled = false;

            }
        }

        //private void OnDataRecieved(object sender, SerialDataReceivedEventArgs e)
        //{
        //    serial.DataReceived -= OnDataRecieved;
        //    Thread.Sleep(10);
        //    serial.Read(buffer, 0, 1024 * 1024);
        //    //收到TCP消息或uart消息
        //    Dispatcher.Invoke(() =>
        //    {
        //        LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到长度为" + length.ToString() + "的数据。\n";
        //    });

        //    if (upperProtocol == UpperProtocol.Customized)
        //    {
        //        switch (buffer[0])
        //        {
        //            case 1://ADC传来的值
        //                int c = 0;
        //                for (int i = 0; i < 1024 * 1024; i++)
        //                {
        //                    if (c > 1)
        //                        buffer[i] = 0;
        //                    else if (buffer[i] == 1)
        //                    {
        //                        buffer[i] = 0;
        //                        c++;
        //                    }
        //                }
        //                AdcString = Encoding.ASCII.GetString(buffer.Skip(1).ToArray());
        //                //AdcString.
        //                AdcData = Convert.ToInt32(AdcString);
        //                Dispatcher.Invoke(() =>
        //                {
        //                    ADCDataTextbox.Text = "ADC值：" + AdcString;
        //                    AdcDataBar.Value = AdcData;
        //                    AdcDatas.AddSequenceData(AdcData);
        //                    AdcGraph.HotspotDatas = AdcDatas;
        //                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到了ADC传来的数据：" + AdcString + "。\n";
        //                });
        //                break;
        //            case 2://Camera数据
        //                if (length == 640 * 480 * 3 + 1)//接受不完整，便丢弃
        //                {
        //                    CameraData = new WriteableBitmap(640, 480, 400, 400, PixelFormats.Rgb24, null);
        //                    //unsafe
        //                    {
        //                        CameraData.Lock();
        //                        Marshal.Copy(buffer.Skip(1).ToArray(), 0, CameraData.BackBuffer, 640 * 480 * 3); //请注意_wbBitmap的数据格式以及buffer大小，以免溢出和显示异常
        //                        CameraData.AddDirtyRect(new Int32Rect(0, 0, 640, 480));
        //                        CameraData.Unlock();
        //                    }
        //                    Dispatcher.Invoke(() =>
        //                    {
        //                        CameraImage.Source = CameraData;
        //                        LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到相机数据，已显示。\n";
        //                    });
        //                }
        //                else
        //                    Dispatcher.Invoke(() =>
        //                    {
        //                        LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到不完整相机数据，已丢弃。\n";
        //                    });
        //                break;
        //            default:
        //                Dispatcher.Invoke(() =>
        //                {
        //                    LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到了不认识的数据，已丢弃。\n";
        //                });
        //                break;
        //        }
        //    }
        //    serial.DataReceived += OnDataRecieved;

        //}
        string tmp;
        private void OnRecieve(IAsyncResult ar)
        {
            //Console.WriteLine("OnReceive Called");
            int c = 0;
            //收到TCP消息或uart消息
            if (lowerProtocol == LowerProtocol.Ethernet)
                try
                {
                    length = client.Client.EndReceive(ar);
                }
                catch (Exception exception)
                {
                    MessageBox.Show(exception.Message);
                    return;
                }

            else if (lowerProtocol == LowerProtocol.UART)
            {
                Thread.Sleep(100);
                length = serial.BaseStream.EndRead(ar);
            }
            else
                throw new Exception("不知道该从哪里read了！");
            Dispatcher.Invoke(() =>
            {
                LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到长度为" + length.ToString() + "的数据。\n";
            });

            if (upperProtocol == UpperProtocol.Customized)
            {
                switch (buffer[0])
                {
                    case 255://控制信息，断开连接
                        if (buffer[1] == 'e' && buffer[2] == 'x' && buffer[3] == 'i' && buffer[4] == 't')
                        {
                            if (lowerProtocol == LowerProtocol.Ethernet)
                                try
                                {
                                    client.Client.Close();
                                }
                                catch (Exception exception)
                                {
                                    MessageBox.Show(exception.Message);
                                    return;
                                }
                            Dispatcher.Invoke(() =>
                            {
                                LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 断开与下位机的UART连接。\n";
                                StartTrans.Content = "连接下位机";
                                ConnectionState = 0;
                                CameraDynamicRadioButton.IsEnabled = false;
                                CameraStaticRadioButton.IsEnabled = false;
                                CameraStopRadioButton.IsEnabled = false;
                                CameraDynamicDetectionRadioButton.IsEnabled = false;//没开发出来
                                AdcStopRadioButton.IsEnabled = false;
                                AdcStaticRadioButton.IsEnabled = false;
                                AdcDynamicRadioButton.IsEnabled = false;
                            });
                            return;
                        }

                        break;
                    case 1://ADC传来的值
                        c = 0;
                        for (int i = 0; i < 1024 * 1024; i++)
                        {
                            if (c > 1)
                                buffer[i] = 0;
                            else if (buffer[i] > '9' || buffer[i] < '0')
                            {
                                buffer[i] = 0;
                                c++;
                            }
                        }
                        c = 0;
                        for (int i = 0; i<2; c++)
                            if (buffer[c] == 0)
                                i++;
                        AdcString = Encoding.ASCII.GetString(buffer.Skip(1).ToArray());
                        tmp = "";
                        for (int i = 0; AdcString[i]!=0; i++)
                            tmp += AdcString[i];
                        
                        if (tmp.Length == 0)
                            break;
                        AdcData = Convert.ToInt32(tmp);
                        Dispatcher.Invoke(() =>
                        {
                            ADCDataTextbox.Text = tmp;
                            AdcDataBar.Value = AdcData;
                            AdcDatas.AddSequenceData(AdcData);
                            AdcGraph.HotspotDatas = AdcDatas;
                            LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到了ADC传来的数据：" + tmp + "。\n";
                            adcDataset.Add(new AdcDatasetItem(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:ms", new System.Globalization.CultureInfo("zh-cn")), AdcData));
                        });
                        break;
                    case 2://Camera数据
                        cnt++;
                        string path = @".\tmp\";
                        string filename = path + "tmp"+cnt+".jpg";

                        fs.Close();
                        fs = new FileStream(filename, FileMode.Create, FileAccess.ReadWrite, FileShare.ReadWrite);
                        fs.Write(buffer, 1, length - 1);

                        //所有流类型都要关闭流，否则会出现内存泄露问题
                        Dispatcher.Invoke(() =>
                        {
                            fs.Close();
                            //BitmapImage ImageSource =
                            CameraImage.Source = new BitmapImage(new Uri(System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + filename));
                            LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到相机数据，已显示。\n";
                        });
                        break;
                    case 3://控制信息，adc发送间隔
                        int GraphBufferLength = (int)(20 * (10.0 / buffer[1]));
                        GraphBufferLength = GraphBufferLength > 20 ? GraphBufferLength : 20;
                        AdcDatas.maxCount = GraphBufferLength;
                        Dispatcher.Invoke(() =>
                        {
                            AdcIntervalTextBox.Text = buffer[1].ToString();
                        });
                        break;
                    case 4://控制信息，adc停止
                        Dispatcher.Invoke(() =>
                        {
                            AdcStopRadioButton.IsChecked = true;
                            SetAdcInterval.IsEnabled = false;
                        });
                        break;
                    case 5://控制信息，adc固定间隔发送
                        Dispatcher.Invoke(() =>
                        {
                            AdcStaticRadioButton.IsChecked = true;
                            SetAdcInterval.IsEnabled = false;
                        });
                        break;
                    case 6://控制信息，adc动态间隔发送
                        Dispatcher.Invoke(() =>
                        {
                            AdcDynamicRadioButton.IsChecked = true;
                            SetAdcInterval.IsEnabled = true;
                        });
                        break;
                    case 7://控制信息，相机发送间隔
                        Dispatcher.Invoke(() =>
                        {
                            CameraIntervalTextBox.Text = buffer[1].ToString();
                        });
                        break;
                    case 8://控制信息，相机停止发送
                        Dispatcher.Invoke(() =>
                        {
                            CameraStopRadioButton.IsChecked = true;
                            SetCameraInterval.IsEnabled = false;
                        });
                        break;
                    case 9://控制信息，相机固定间隔发送
                        Dispatcher.Invoke(() =>
                        {
                            CameraStaticRadioButton.IsChecked = true;
                            SetCameraInterval.IsEnabled = false;
                        });
                        break;
                    case 10://控制信息，相机动态间隔发送
                        Dispatcher.Invoke(() =>
                        {
                            CameraDynamicRadioButton.IsChecked = true;
                            SetCameraInterval.IsEnabled = true;
                        });
                        break;
                    case 11://
                        break;
                    case 12:
                        break;
                    case 13:
                        break;
                    default:
                        Dispatcher.Invoke(() =>
                        {
                            LogTextBox.Text += "[" + DateTime.Now.ToString("HH:mm:ss", new System.Globalization.CultureInfo("zh-cn")) + "] 收到了不认识的数据，已丢弃。\n";
                        });
                        break;
                }
            }
            buffer = new byte[1024 * 1024];
            if (lowerProtocol == LowerProtocol.Ethernet)
                client.Client.BeginReceive(buffer, 0, 1024 * 1024, SocketFlags.None, OnRecieve, null);
            else if (lowerProtocol == LowerProtocol.UART)
                serial.BaseStream.BeginRead(buffer, 0, 1024 * 1024, OnRecieve, null);

        }

        private void SaveCameraImage_Click(object sender, RoutedEventArgs e)
        {
            //CameraData.
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

        private void SetAdcInterval_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[2];
            buffer[0] = 3;
            buffer[1] = (byte)Convert.ToUInt32(AdcIntervalTextBox.Text);
            client.Client.Send(buffer);
        }

        private void AdcStopRadioButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[1];
            buffer[0] = 4;
            client.Client.Send(buffer);
            SetAdcInterval.IsEnabled = false;
        }

        private void AdcStaticRadioButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[1];
            buffer[0] = 5;
            client.Client.Send(buffer);
            SetAdcInterval.IsEnabled = false;

        }

        private void AdcDynamicRadioButton_Click(object sender, RoutedEventArgs e)
        {
            SetAdcInterval.IsEnabled = true;
            byte[] buffer = new byte[1];
            buffer[0] = 6;
            client.Client.Send(buffer);
            buffer = new byte[2];
            buffer[0] = 3;
            buffer[1] = (byte)Convert.ToUInt32(AdcIntervalTextBox.Text);
            client.Client.Send(buffer);

        }

        private void SetCameraInterval_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[2];
            buffer[0] = 7;
            buffer[1] = (byte)Convert.ToUInt32(AdcIntervalTextBox.Text);
            client.Client.Send(buffer);
 

        }

        private void CameraStopRadioButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[1];
            buffer[0] = 8;
            client.Client.Send(buffer);
            SetCameraInterval.IsEnabled = false;

        }

        private void CameraStaticRadioButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[1];
            buffer[0] = 9;
            client.Client.Send(buffer);
            SetCameraInterval.IsEnabled = false;
        }

        private void CameraDynamicRadioButton_Click(object sender, RoutedEventArgs e)
        {
            SetCameraInterval.IsEnabled = true;
            byte[] buffer = new byte[1];
            buffer[0] = 10;
            client.Client.Send(buffer);
            buffer = new byte[2];
            buffer[0] = 7;
            buffer[1] = (byte)Convert.ToUInt32(AdcIntervalTextBox.Text);
            client.Client.Send(buffer);

        }

        private void CameraDynamicDetectionRadioButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[1];
            buffer[0] = 11;
            client.Client.Send(buffer);
            SetCameraInterval.IsEnabled = false;

        }

        private void SaveCameraGraph_Click(object sender, RoutedEventArgs e)
        {
            string sourcePath = @".\tmp\";
            string sourceFilename = sourcePath + "tmp" + cnt + ".jpg";
            string targetPath = @".\SavedImg\";//指定存储的路径
            string targetFilename = targetPath + DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss") + ".jpg";


            if (File.Exists(sourceFilename))//必须判断要复制的文件是否存在
            {
                if (!Directory.Exists(targetPath))
                {
                    Directory.CreateDirectory(targetPath);
                }
                File.Copy(sourceFilename, targetFilename, true);//三个参数分别是源文件路径，存储路径，若存储路径有相同文件是否替换
            }
        }

        private void ViewCameraData_Click(object sender, RoutedEventArgs e)
        {
            string strDir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + @"\SavedImg\";
            if (!Directory.Exists(strDir))
            {
                Directory.CreateDirectory(strDir);
            }

            System.Diagnostics.Process.Start("explorer.exe", strDir);
        }

        private void ViewCurrentADCData_Click(object sender, RoutedEventArgs e)
        {
            ShowAdcDataset window = new ShowAdcDataset();
            window.AdcDataset = adcDataset;
            window.Show();
        }

        private void ViewOldADCData_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "ADS Binary File(*.ads)|*.ads";
            openFileDialog.DefaultExt = ".ads"; // Default file extension
            string strDir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + @"\AdcDataset\";

            if (!Directory.Exists(strDir))
            {
                Directory.CreateDirectory(strDir);
            }

            openFileDialog.InitialDirectory = strDir;
            if (openFileDialog.ShowDialog() == true)//注意，此处一定要手动引入System.Window.Forms空间，否则你如果使用默认的DialogResult会发现没有OK属性
            {
                ShowAdcDataset window = new ShowAdcDataset();
                window.AdcDataset = AdcDataset.ReadFrom(openFileDialog.FileName);
                window.Show();

            }
        }

    private void SaveCurrentADCData_Click(object sender, RoutedEventArgs e)
        {
            string strDir = @".\AdcDataset\";
            string strpath = strDir + DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss") + ".ads";
            if (!Directory.Exists(strDir))
            {
                Directory.CreateDirectory(strDir);
            }

            adcDataset.SaveTo(strpath);

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
