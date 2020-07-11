#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adc_page.h"
#include "qt1.h"
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
