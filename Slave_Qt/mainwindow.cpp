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
    //UARTͨ�ŷ�ʽ
}

void MainWindow::on_rb_Ethernet_clicked()
{
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
    //���ֹͣ�ɼ��ͷ�������
}

void MainWindow::on_rb_camera_static_clicked()
{
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
    //ADCֹͣ�ɼ��ͷ�������
}

void MainWindow::on_rb_adc_static_clicked()
{
    //ADC�Թ̶����ʲɼ��ͷ���
}

void MainWindow::on_rb_adc_dynamic_clicked()
{
    //ADC�Զ�̬���ʲɼ��ͷ��ͣ�������sb_adc_freq������
}

void MainWindow::on_sb_camera_freq_valueChanged(int camera_freq)
{
    //�����̬�ɼ����ʸı���Ӧ����
}

void MainWindow::on_sb_adc_freq_valueChanged(int adc_freq)
{
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

     //���յ������ݵ��ź��Լ����ӶϿ����ź�
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
    //��ȡ����������
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




