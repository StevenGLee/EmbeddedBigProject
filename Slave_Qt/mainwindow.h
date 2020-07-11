#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "qt1.h"
#include "adc_page.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_rb_UART_clicked();

    void on_rb_Ethernet_clicked();

    void on_rb_modbus_clicked();

    void on_rb_mqtt_clicked();

    void on_rb_camera_stop_clicked();

    void on_rb_camera_static_clicked();

    void on_rb_camera_dynamic_clicked();

    void on_rb_camera_dynamic_monitor_clicked();

    void on_rb_adc_stop_clicked();

    void on_rb_adc_static_clicked();

    void on_rb_adc_dynamic_clicked();

    void on_sb_camera_freq_valueChanged(int );

    void on_sb_adc_freq_valueChanged(int );

    void on_pb_start_sending_clicked();

    void on_pb_exit_clicked();

    void on_pb_camera_page_clicked();

    void on_pb_adc_page_clicked();

private:
    QButtonGroup* SendingButtons;
    QButtonGroup* CameraButtons;
    QButtonGroup* ADCButtons;
    Ui::MainWindow *ui;
    int isSending;
    //Qt1 *camera_page;
    //ADC_page *adc_page;

};

#endif // MAINWINDOW_H
