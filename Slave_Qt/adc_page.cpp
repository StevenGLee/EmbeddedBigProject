#include "adc_page.h"
#include "ui_adc_page.h"
#include "mainwindow.h"
#include "qt1.h"
extern Qt1* camera_page;
extern ADC_page* adc_page;
extern MainWindow* main_window;

ADC_page::ADC_page(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ADC_page)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    the_parent = parent;
}

ADC_page::~ADC_page()
{
    delete ui;
}

void ADC_page::on_pushButton_2_clicked()
{
    main_window->show();
    hide();
}
