#include "adc_page.h"
#include "ui_adc_page.h"
#include "mainwindow.h"
#include "qt1.h"
#include "GraphItem.h"
#include <QGraphicsScene>

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
    ui->progressBar->setRange(0, 10000);
    ui->progressBar->setTextVisible(false);
    QGraphicsScene *m_scene=new QGraphicsScene;
    item=new GraphItem;
    item->maxCount = 20;
    item->maxValue = 10000;
    item->minValue = 0;
    item->setPos(0,0);
    item->setColor(QColor(135,206,250));
    m_scene->addItem(item);
    ui->graphicsView->setSceneRect(-361/2,-231/2,361,231);
    ui->graphicsView->setScene(m_scene);
    last_scene = m_scene;
}

void ADC_page::update_data(int value)
{
    ui->lcdNumber->display(value);
    ui->progressBar->setValue(value);
    item->addValue(value);
    printf("update_data()\n");
    QGraphicsScene *m_scene=new QGraphicsScene;
    m_scene->addItem(item);
    ui->graphicsView->setSceneRect(-361/2,-231/2,361,231);
    ui->graphicsView->setScene(m_scene);
    delete last_scene;
    last_scene = m_scene;

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
