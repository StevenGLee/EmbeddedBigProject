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
