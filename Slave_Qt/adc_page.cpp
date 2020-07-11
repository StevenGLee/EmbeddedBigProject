#include "adc_page.h"
#include "ui_adc_page.h"

ADC_page::ADC_page(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ADC_page)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

ADC_page::~ADC_page()
{
    delete ui;
}
