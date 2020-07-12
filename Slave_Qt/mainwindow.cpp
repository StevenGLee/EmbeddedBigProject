#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adc_page.h"
#include "qt1.h"
#include"camera.h"
#include"ad_reader.h"
#include <QDebug>
#include <QFile>


extern Qt1* camera_page;
extern ADC_page* adc_page;
extern MainWindow* main_window;
MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);

    //adc_page = new ADC_page(this);
    //camera_page = new Qt1(this);
    //adc_page -> hide();
    //camera_page -> hide();
    SendingButtons = new QButtonGroup(this);
    SendingButtons->addButton(ui->rb_UART);
    SendingButtons->addButton(ui->rb_Ethernet);
    SendingButtons->addButton(ui->rb_modbus);
    SendingButtons->addButton(ui->rb_mqtt);
    SendingButtons->setExclusive(true);
    ui->rb_UART->setChecked(true);

    ui->rb_modbus->setEnabled(false);
    ui->rb_mqtt->setEnabled(false);

    CameraButtons = new QButtonGroup(this);
    CameraButtons->addButton(ui->rb_camera_stop);
    CameraButtons->addButton(ui->rb_camera_static);
    CameraButtons->addButton(ui->rb_camera_dynamic);
    CameraButtons->addButton(ui->rb_camera_dynamic_monitor);
    CameraButtons->setExclusive(true);
    ui->rb_camera_stop->setChecked(true);

    ui->rb_camera_dynamic_monitor->setEnabled(false);

    ADCButtons = new QButtonGroup(this);
    ADCButtons->addButton(ui->rb_adc_stop);
    ADCButtons->addButton(ui->rb_adc_static);
    ADCButtons->addButton(ui->rb_adc_dynamic);
    ui->rb_adc_stop->setChecked(true);

    isSending = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete SendingButtons;
    delete CameraButtons;
    delete ADCButtons;
}

void MainWindow::on_rb_UART_clicked()
{
    //UART通信方式
}

void MainWindow::on_rb_Ethernet_clicked()
{
    //Ethernet通信方式
}

void MainWindow::on_rb_modbus_clicked()
{
    //modbus通信方式
}

void MainWindow::on_rb_mqtt_clicked()
{
    //mqtt通信方式
}

void MainWindow::on_rb_camera_stop_clicked()
{
    //相机停止采集和发送数据
}

void MainWindow::on_rb_camera_static_clicked()
{
    //相机以固定速率采集和发送
}

void MainWindow::on_rb_camera_dynamic_clicked()
{
    //相机以动态速率采集和发送，速率在sb_camera_freq中设置
}

void MainWindow::on_rb_camera_dynamic_monitor_clicked()
{
    //相机以固定速率采集，并以动态监测的方式发送
}

void MainWindow::on_rb_adc_stop_clicked()
{
    //ADC停止采集和发送数据
}

void MainWindow::on_rb_adc_static_clicked()
{
    //ADC以固定速率采集和发送
}

void MainWindow::on_rb_adc_dynamic_clicked()
{
    //ADC以动态速率采集和发送，速率在sb_adc_freq中设置
}

void MainWindow::on_sb_camera_freq_valueChanged(int camera_freq)
{
    //相机动态采集速率改变响应函数
}

void MainWindow::on_sb_adc_freq_valueChanged(int adc_freq)
{
    //adc动态采集速率改变响应函数
}

void MainWindow::on_pb_start_sending_clicked()
{
    //“开始发送”和“停止发送”按钮点击事件
    if(isSending == 0)//不在发送时点击，说明开始发送
    {
        ui -> rb_UART -> setEnabled(false);
        ui -> rb_Ethernet -> setEnabled(false);
        //ui -> rb_modbus -> setEnabled(false);
        //ui -> rb_mqtt -> setEnabled(false);
        isSending = 1;
        ui -> pb_start_sending->setText("Stop Sending");
    }
    else
    {
        ui -> rb_UART -> setEnabled(true);
        ui -> rb_Ethernet -> setEnabled(true);
        //ui -> rb_modbus -> setEnabled(true);
        //ui -> rb_mqtt -> setEnabled(true);
        isSending = 0;
        ui -> pb_start_sending->setText("Start Sending");

    }
}

void MainWindow::on_pb_exit_clicked()
{
    //”退出程序“按钮点击事件
    close();
}

void MainWindow::on_pb_camera_page_clicked()
{
    //“进入相机页面”以查看当前正在采集的图像，以及查看已经保存的图像
    hide();
    camera_page->show();
    //show();
}

void MainWindow::on_pb_adc_page_clicked()
{
    //“进入ADC页面”以查看记录的adc数据曲线，以及保存当前现实的数据曲线
    hide();
    adc_page->show();
    //show();
}

void MainWindow::TcpServer()
{
    tcpserver = new QTcpServer();
    connect(tcpserver, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}
int MainWindow::run()
{
    if (tcpserver->listen(QHostAddress::Any, 3230))
       {
           qDebug()<<"[TcpServer]-------------------------------------------------listen sucess"<<endl;
       }
       else
       {
           qDebug()<<"[TcpServer]-------------------------------------------------listen faile"<<endl;
       }

}

void MainWindow::newConnectionSlot()
{
     qDebug()<<"[TcpServer]-------------------------------------------------new Connection !!!"<<endl;
     tcpsocket = tcpserver->nextPendingConnection();
     qDebug()<<"From ---> "<<tcpsocket->peerAddress()<<":"<<tcpsocket->peerPort()<<endl;


     Qstring str=buffer_send;
     tcpsocket->write(str.toUtf8().data());

     //接收到新数据的信号以及连接断开的信号
     connect(tcpsocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));
     connect(tcpsocket, SIGNAL(disconnected()), tcpsocket, SLOT(deleteLater()));
}

char MainWindow::ChangeSampleTime(int milliseconds)
{

    ad_reader AD;
    AD.init();
    t5.start(milliseconds);
    ad_zhi=AD.ad();

    //QString filepath = "./images/"; //"/mnt/usb/images/";
    //QDateTime d=QDateTime::currentDateTime();
    //filepath += d.toString("MM-dd_hh-mm-ss");
    //filepath += ".jpg";
    //m_image->save(filepath, "JPG", -1);

    Qt1 Cam;
    Cam.fun_take_photo();
    RGB_zhi=Cam.frameBufRGB;
    sprintf(buffer_send,"%d\n %hhu\n",ad_zhi,RGB_zhi);
    return buffer_send;

}

void MainWindow::dataReceived()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = tcpsocket->readAll();
    if(!buffer.isEmpty())
    {
        QString command =  QString::fromLocal8Bit(buffer);
        qDebug()<<"[command]:" << command <<endl;
    }
    ui->textEditRead->append(buffer);

}


void MainWindow::SendM()
{
    printf("Choose send way:\n1. UART (default)\n2. socket\n");
    scanf("%d", &func);
    switch(func)
    {
    case 1:
        char buffer_send[MAX_SIZE];


        uart3 = "/dev/ttySAC3";

        fd = open(uart3, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1)
        {
                printf("open %s is failed", dest);
                return 0;
        }
        set_opt(fd, 115200, 8, 'N', 1);
        while(1){
            printf("send:%s\n", buffer_send);
            write(fd, buffer_send, strlen(buffer_send));
        }


      case 2:
        TcpServer();
        run();
        newConnectionSlot();
        while(1){
            dataReceived();
        }
    }

}

int MainWindow::set_opt(int fd_uart,int nSpeed, int nBits, char nEvent, int nStop)
{
        struct termios newtio,oldtio;
        //tcgetarr读取当前串口的参数值，fd是open返回的句柄
        if  ( tcgetattr( fd_uart,&oldtio)  !=  0) {
                perror("SetupSerial 1");
                return -1;
        }
        bzero( &newtio, sizeof( newtio ) );

        //设置字符大小
        newtio.c_cflag  |=  CLOCAL | CREAD;
        newtio.c_cflag &= ~CSIZE;

        switch( nBits )
        {
                //设置停止位
                case 7:
                        newtio.c_cflag |= CS7;
                        break;
                case 8:
                        newtio.c_cflag |= CS8;
                        break;
        }
        //设置奇偶校验位
        switch( nEvent )
        {
        case 'O':
                newtio.c_cflag |= PARENB;
                newtio.c_cflag |= PARODD;
                newtio.c_iflag |= (INPCK | ISTRIP);
                break;
        case 'E':
                newtio.c_iflag |= (INPCK | ISTRIP);
                newtio.c_cflag |= PARENB;
                newtio.c_cflag &= ~PARODD;
                break;
        case 'N':
                newtio.c_cflag &= ~PARENB;
                break;
        }
        //设置波特率
        switch( nSpeed )
        {
                case 2400:
                        cfsetispeed(&newtio, B2400);
                        cfsetospeed(&newtio, B2400);
                        break;
                case 4800:
                        cfsetispeed(&newtio, B4800);
                        cfsetospeed(&newtio, B4800);
                        break;
                case 9600:
                        cfsetispeed(&newtio, B9600);
                        cfsetospeed(&newtio, B9600);
                        break;
                case 115200:
                        cfsetispeed(&newtio, B115200);
                        cfsetospeed(&newtio, B115200);
                        break;
                case 460800:
                        cfsetispeed(&newtio, B460800);
                        cfsetospeed(&newtio, B460800);
                        break;
                default:
                        cfsetispeed(&newtio, B9600);
                        cfsetospeed(&newtio, B9600);
                        break;
        }
        if( nStop == 1 )
                newtio.c_cflag &=  ~CSTOPB;
        else if ( nStop == 2 )
                newtio.c_cflag |=  CSTOPB;
                newtio.c_cc[VTIME]  = 0;
                newtio.c_cc[VMIN] = 0;
                tcflush(fd_uart,TCIFLUSH);
        if((tcsetattr(fd_uart,TCSANOW,&newtio))!=0)
        {
                perror("com set error");
                return -1;
        }

        //	printf("set done!\n\r");
        return 0;
}




