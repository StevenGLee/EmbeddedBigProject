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
    isuart=0;
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
        //UARTͨ�ŷ�ʽ
}

void MainWindow::on_rb_Ethernet_clicked()
{
    isServer==1;

    //Ethernetͨ�ŷ�ʽ
}

void MainWindow::on_rb_modbus_clicked()
{
    //modbusͨ�ŷ�ʽ
}

void MainWindow::on_rb_mqtt_clicked()
{
    //mqttͨ�ŷ�ʽ
}

void MainWindow::on_rb_camera_stop_clicked()
{

    camera_page->isCapOpen=1;
    camera_page->fun_cap_open();
    //���ֹͣ�ɼ��ͷ�������
}

void MainWindow::on_rb_camera_static_clicked()
{
    camera_page->t4.stop();
    camera_page->t4.start(1000);
    camera_page->isCapOpen=0;
    camera_page->fun_cap_open();
    //����Թ̶����ʲɼ��ͷ���
}

void MainWindow::on_rb_camera_dynamic_clicked()
{
    //����Զ�̬���ʲɼ��ͷ��ͣ�������sb_camera_freq������
}

void MainWindow::on_rb_camera_dynamic_monitor_clicked()
{
    //����Թ̶����ʲɼ������Զ�̬���ķ�ʽ����
}

void MainWindow::on_rb_adc_stop_clicked()
{
    camera_page->t5.stop();
    AD.close();
    //ADCֹͣ�ɼ��ͷ�������
}



void MainWindow::on_rb_adc_static_clicked()
{
    AD.open();
    camera_page->t5.start(1000);
    main_window->SendADC(AD.ad());
    //ADC�Թ̶����ʲɼ��ͷ���
}

void MainWindow::on_rb_adc_dynamic_clicked()
{

    //ADC�Զ�̬���ʲɼ��ͷ��ͣ�������sb_adc_freq������
}

void MainWindow::on_sb_camera_freq_valueChanged(int camera_freq)
{
    camera_page->t4.stop();
    camera_page->isCapOpen=0;
    camera_page->change_time(camera_freq);
    camera_page->fun_cap_open();

    //camera_page->isCapOpen=0;
    //�����̬�ɼ����ʸı���Ӧ����
}

void MainWindow::on_sb_adc_freq_valueChanged(int adc_freq)
{
    camera_page->t5.stop();
    camera_page->t5.start(adc_freq);
    main_window->SendADC(AD.ad());

    //adc��̬�ɼ����ʸı���Ӧ����
}

void MainWindow::on_pb_start_sending_clicked()
{
    //����ʼ���͡��͡�ֹͣ���͡���ť����¼�
    if(isSending == 0)//���ڷ���ʱ�����˵����ʼ����
    {
        ui -> rb_UART -> setEnabled(false);
        ui -> rb_Ethernet -> setEnabled(false);
        //ui -> rb_modbus -> setEnabled(false);
        //ui -> rb_mqtt -> setEnabled(false);
        isSending = 1;
        ui -> pb_start_sending->setText("Stop Sending");
        if(ui->rb_UART->isChecked())
        {
            if(isServer==0&&isuart==1)
            {
                fd = ::open(uart3, O_RDWR | O_NOCTTY | O_NDELAY);
                if (fd == -1)
                {
                    printf("open %s is failed", uart3);

                }
                set_opt(fd, 115200, 8, 'N', 1);
            }
        }
        else if(ui->rb_Ethernet->isChecked())
        {
            //����ط�һ����֪����д�Ĳ��ԡ��������Լ��ġ�
            if(isServer==1&&isuart==0)
            {
                TcpServer();
                run();
            }

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
            isuart=0;
            //����ֱ�ӹر��˴��ڡ�ע�������߼���ʹ�رմ��ں����дӴ����ߵ����ݷ��Ͷ�������
            //�����ڷ��͵�ʱ���ж�ui->rb_UART->isChecked()Ϊ�棬��isSending==1ʱ�Ž���send����write
        }
        else if(ui->rb_Ethernet->isChecked())
        {
            isServer=0;
            tcpsocket->write("exit");
            tcpserver->close();
            //�����ֹͣ���͡����ر����ӣ����ر�server
            //�ر�����ǰ�ȷ��͡�exit������ַ�����֪ͨ��λ���ر�����
        }

    }
}

void MainWindow::on_pb_exit_clicked()
{
    //���˳����򡰰�ť����¼�
    close();
}

void MainWindow::on_pb_camera_page_clicked()
{
    //���������ҳ�桱�Բ鿴��ǰ���ڲɼ���ͼ���Լ��鿴�Ѿ������ͼ��
    hide();
    camera_page->show();
    //show();
}

void MainWindow::on_pb_adc_page_clicked()
{
    //������ADCҳ�桱�Բ鿴��¼��adc�������ߣ��Լ����浱ǰ��ʵ����������
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


     //QString str=buffer_send;


     //���յ������ݵ��ź��Լ����ӶϿ����ź�
     connect(tcpsocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));
     connect(tcpsocket, SIGNAL(disconnected()), tcpsocket, SLOT(deleteLater()));
}



void MainWindow::dataReceived()
{
    QByteArray buffer;
    //��ȡ����������
    buffer = tcpsocket->readAll();
    if(!buffer.isEmpty())
    {
        QString command =  QString::fromLocal8Bit(buffer);
        qDebug()<<"[command]:" << command <<endl;
    }
    printf("send:%s\n",buffer);

}


void MainWindow::SendRGB(unsigned char *frameBufRGB)
{
    memset(buffer_send, 0, sizeof(buffer_send));
    buffer_send[0]=2;
    //const char* RGB = (const char*)(char*)frameBufRGB;
    //sprintf(buffer_send+1, "%s", *frameBufRGB);
    memcpy(buffer_send+1,frameBufRGB,640*480*3);
    //write(fd, buffer_send, 640*480*3+1);
    if(isServer==1&&isuart==0)
    {
        tcpsocket->write(buffer_send,strlen(buffer_send));

    }

}

void MainWindow::SendADC(int ad)
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
    //char *buf;
    //sprintf(buf,"%d",AD.ad());
    //memcpy(buffer_send,buf,(strlen(buf) + 1));
}



int MainWindow::set_opt(int fd_uart,int nSpeed, int nBits, char nEvent, int nStop)
{
        struct termios newtio,oldtio;
        //tcgetarr��ȡ��ǰ���ڵĲ���ֵ��fd��open���صľ��
        if  ( tcgetattr( fd_uart,&oldtio)  !=  0) {
                perror("SetupSerial 1");
                return -1;
        }
        bzero( &newtio, sizeof( newtio ) );

        //�����ַ���С
        newtio.c_cflag  |=  CLOCAL | CREAD;
        newtio.c_cflag &= ~CSIZE;

        switch( nBits )
        {
                //����ֹͣλ
                case 7:
                        newtio.c_cflag |= CS7;
                        break;
                case 8:
                        newtio.c_cflag |= CS8;
                        break;
        }
        //������żУ��λ
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
        //���ò�����
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




