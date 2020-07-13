#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adc_page.h"
#include "qt1.h"
#include"camera.h"
#include"ad_reader.h"
#include"mainwindow.h"
#include <QDebug>
#include <QFile>
#include<fcntl.h>
#include<sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include<string.h>
#define MAXSIZE 1048576

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
    isServer=0;
    isuart=1;
    isAdcOpen = 0;
    uart3 = "/dev/ttySAC3";

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
    isuart=1;
    isServer=0;
        //UART通信方式
}

void MainWindow::on_rb_Ethernet_clicked()
{
    isServer=1;
    isuart=0;

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
    if(camera_page->isCapOpen == 1)
        camera_page->fun_cap_open();
        //相机停止采集和发送数据
    char buffer[1];
    buffer[0] = 8;
    if(isSending)
        if(isServer==0&&isuart==1)
        {

        }
        else if(isServer==1&&isuart==0)
        {
            tcpsocket->write(buffer,1);
        }

}

void MainWindow::on_rb_camera_static_clicked()
{
    if(camera_page->isCapOpen == 0)
        camera_page->fun_cap_open();
    else
        camera_page->t4.stop();
    camera_page->t4.start(1000);
    char buffer[1];
    buffer[0] = 9;
    if(isSending)
        if(isServer==0&&isuart==1)
        {

        }
        else if(isServer==1&&isuart==0)
        {
            tcpsocket->write(buffer,1);
        }

    //相机以固定速率采集和发送
}

void MainWindow::on_rb_camera_dynamic_clicked()
{
    //相机以动态速率采集和发送，速率在sb_camera_freq中设置
    if(camera_page->isCapOpen == 0)
        camera_page->fun_cap_open();
    else
        camera_page->t4.stop();
    camera_page->t4.start(ui->sb_camera_freq->value() * 100);
    char buffer[1];
    buffer[0] = 10;
    char buffer2[2];
    buffer[0] = 7;
    buffer[1] = ui->sb_camera_freq->value();

    if(isSending)
        if(isServer==0&&isuart==1)
        {

        }
        else if(isServer==1&&isuart==0)
        {
            tcpsocket->write(buffer,1);
            usleep(10000);
            tcpsocket->write(buffer2,2);
        }

}

void MainWindow::on_rb_camera_dynamic_monitor_clicked()
{
    //相机以固定速率采集，并以动态监测的方式发送
}

void MainWindow::on_rb_adc_stop_clicked()
{
    if(isAdcOpen == 1)
    {
        camera_page->t5.stop();
        AD.close();
        isAdcOpen = 0;
        //ADC停止采集和发送数据
    }
    char buffer[1];
    buffer[0] = 4;
    if(isSending)
        if(isServer==0&&isuart==1)
        {

        }
        else if(isServer==1&&isuart==0)
        {
            tcpsocket->write(buffer,1);
        }
}



void MainWindow::on_rb_adc_static_clicked()
{
    if(isAdcOpen == 0)
    {
        AD.open();
        isAdcOpen = 1;
    }
    else
        camera_page->t5.stop();
    if(isSending)
    {
        char buffer[1];
        buffer[0] = 5;
        if(isServer==0&&isuart==1)
        {

        }
        else if(isServer==1&&isuart==0)
        {
            tcpsocket->write(buffer,1);
        }
    }

    camera_page->t5.start(1000);
    //ADC以固定速率采集和发送
}

void MainWindow::on_rb_adc_dynamic_clicked()
{
    if(isAdcOpen == 0)
    {
        AD.open();
        isAdcOpen = 1;
    }
    else
        camera_page->t5.stop();
    if(isSending)
    {
        char buffer[1];
        buffer[0] = 6;
        char buffer2[2];
        buffer[0] = 3;
        buffer[1] = ui->sb_adc_freq->value();
        if(isServer==0&&isuart==1)
        {

        }
        else if(isServer==1&&isuart==0)
        {
            tcpsocket->write(buffer,1);
            usleep(10000);
            tcpsocket->write(buffer2,2);
        }
    }

    camera_page->t5.start(ui->sb_adc_freq->value()*100);

    //ADC以动态速率采集和发送，速率在sb_adc_freq中设置
}

void MainWindow::on_sb_camera_freq_valueChanged(int camera_freq)
{
    if(camera_page->isCapOpen == 1)
    {
        camera_page->t4.stop();
        camera_page->t4.start(camera_freq*100);
        if(isSending)
        {
            char buffer2[2];
            buffer2[0] = 7;
            buffer2[1] = ui->sb_camera_freq->value();
            if(isServer==0&&isuart==1)
            {

            }
            else if(isServer==1&&isuart==0)
            {
                tcpsocket->write(buffer2,2);
            }
        }
    }
    //camera_page->isCapOpen=0;
    //相机动态采集速率改变响应函数
}

void MainWindow::on_sb_adc_freq_valueChanged(int adc_freq)
{
    if(isAdcOpen == 1)
    {
        camera_page->t5.stop();
        camera_page->t5.start(adc_freq*100);
        if(isSending)
        {
            char buffer2[2];
            buffer2[0] = 3;
            buffer2[1] = ui->sb_adc_freq->value();
            if(isServer==0&&isuart==1)
            {

            }
            else if(isServer==1&&isuart==0)
            {
                tcpsocket->write(buffer2,2);
            }
        }

    }


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
        ui -> pb_start_sending->setText("Stop Sending");
        if(isServer==0&&isuart==1)
        {
            fd = ::open(uart3, O_RDWR | O_NOCTTY | O_NDELAY);
            if (fd == -1)
                printf("open %s is failed", uart3);
            set_opt(fd, 115200, 8, 'N', 1);
            isSending = 1;
        }
        else if(isServer==1&&isuart==0)
        {
            TcpServer();
            run();
        }
    }
    else
    {
        ui -> rb_UART -> setEnabled(true);
        ui -> rb_Ethernet -> setEnabled(true);
        //ui -> rb_modbus -> setEnabled(true);
        //ui -> rb_mqtt -> setEnabled(true);
        isSending = 0;
        ui -> pb_start_sending->setText("Start Sending");
        if(ui->rb_UART->isChecked())
        {
            ::close(fd);
            //这里直接关闭了串口。注意完善逻辑，使关闭串口后，所有从串口走的数据发送都不走了
            //比如在发送的时候，判断ui->rb_UART->isChecked()为真，且isSending==1时才进行send或者write
        }
        else if(ui->rb_Ethernet->isChecked())
        {
            char tmpbuf[5] = {255, 'e','x','i','t'};
            tcpsocket->write(tmpbuf,5);
            tcpserver->close();
            //点击“停止发送”，关闭连接，并关闭server
            //关闭连接前先发送“exit”这个字符串，通知上位机关闭连接
        }

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
void MainWindow::run()
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
     isSending = 1;

     //QString str=buffer_send;


     //接收到新数据的信号以及连接断开的信号
     connect(tcpsocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));
     connect(tcpsocket, SIGNAL(disconnected()), tcpsocket, SLOT(deleteLater()));
     tcpsocket -> write("hello",6);
}



void MainWindow::dataReceived()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = tcpsocket->readAll();
    if(!buffer.isEmpty())
    {
        switch(buffer[0])
        {
        case 255://控制信息，断开连接
            if(buffer[1] == '1' && buffer[2] == 'x' && buffer[3] == 'i' && buffer[4] == 't')
            {
                tcpsocket->close();
                ui -> rb_UART -> setEnabled(true);
                ui -> rb_Ethernet -> setEnabled(true);
                //ui -> rb_modbus -> setEnabled(true);
                //ui -> rb_mqtt -> setEnabled(true);
                isSending = 0;
                ui -> pb_start_sending->setText("Start Sending");
            }
            break;
        case 3://控制信息，adc发送间隔
            ui->sb_adc_freq->setValue(buffer[1]);
            if(isAdcOpen == 1)
            {
                camera_page->t5.stop();
                camera_page->t5.start(buffer[1]*100);
            }
            break;
        case 4://控制信息，adc停止
            ui->rb_adc_stop->setChecked(true);
            if(isAdcOpen == 1)
            {
                camera_page->t5.stop();
                AD.close();
                isAdcOpen = 0;
                //ADC停止采集和发送数据
            }
            break;
        case 5://控制信息，adc固定间隔发送
            ui->rb_adc_static->setChecked(true);

            if(isAdcOpen == 0)
            {
                AD.open();
                isAdcOpen = 1;
            }
            else
                camera_page->t5.stop();
            camera_page->t5.start(1000);
            break;
        case 6://控制信息，adc动态间隔发送
            ui->rb_adc_dynamic->setChecked(true);
            if(isAdcOpen == 0)
            {
                AD.open();
                isAdcOpen = 1;
            }
            else
                camera_page->t5.stop();
            break;
        case 7://控制信息，相机发送间隔
            ui->sb_camera_freq->setValue(buffer[1]);
            if(camera_page->isCapOpen == 1)
            {
                camera_page->t4.stop();
                camera_page->t4.start(buffer[1]*100);
            }

            break;
        case 8://控制信息，相机停止发送
            ui->rb_camera_stop->setChecked(true);
            if(camera_page->isCapOpen == 1)
                camera_page->fun_cap_open();

            break;
        case 9://控制信息，相机固定间隔发送
            ui->rb_camera_static->setChecked(true);

            if(camera_page->isCapOpen == 0)
                camera_page->fun_cap_open();
            else
                camera_page->t4.stop();
            camera_page->t4.start(1000);

            break;
        case 10://控制信息，相机动态间隔发送
            ui->rb_camera_dynamic->setChecked(true);
            if(camera_page->isCapOpen == 0)
                camera_page->fun_cap_open();
            else
                camera_page->t4.stop();

            break;
        case 11://
            break;

        }
    }
    printf("receive:%d\n",buffer.data()[0]);

}


void MainWindow::SendRGB(unsigned char *frameBufRGB)
{
    printf("calling sendRGB\n");

    memset(buffer_send, 0, sizeof(buffer_send));
    printf("memset complete\n");

    QString filepath = "./tmp_images/tmp.jpg"; //"/mnt/usb/images/";
    camera_page -> m_image->save(filepath, "JPG", -1);
    printf("save OK\n");

    int fd = ::open(filepath.toStdString().c_str(), O_RDONLY, 0777);
    printf("Open OK, fd = %d\n", fd);
    int len = ::read(fd, buffer_send + 1, 1024*1024 - 1);
    printf("Read OK, length = %d\n", len);
    ::close(fd);
    //remove(filepath.toStdString().c_str());
    buffer_send[0]=2;
    //const char* RGB = (const char*)(char*)frameBufRGB;
    //sprintf(buffer_send+1, "%s", *frameBufRGB);
    //memcpy(buffer_send+1,frameBufRGB,640*480*3);
    //write(fd, buffer_send, 640*480*3+1);
    if(isServer==1&&isuart==0)
    {
        tcpsocket->write(buffer_send,len+1);
        printf("Socket Write OK\n");

    }
    else if(isServer==0&&isuart==1)
        write(fd, buffer_send, len+1);

}

void MainWindow::SendADC()
{
    ad=AD.ad();
    if(isSending == 1)
    {
        memset(buffer_send, 0, sizeof(buffer_send));
        buffer_send[0]=1;
        sprintf(buffer_send+1, "%d", ad);
        if(isServer==0&&isuart==1)
        {
            write(fd, buffer_send, strlen(buffer_send));

        }
        if(isServer==1&&isuart==0)
        {
            tcpsocket->write(buffer_send,strlen(buffer_send));

        }
    }
    adc_page->update_data(AD.r);

    //char *buf;
    //sprintf(buf,"%d",AD.ad());
    //memcpy(buffer_send,buf,(strlen(buf) + 1));
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
