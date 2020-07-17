#ifndef ADC_PAGE_H
#define ADC_PAGE_H

#include <QDialog>
#include <QPainter>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
//#include <QDebug>
#include <QVector>
#include <QGraphicsScene>
#include "GraphItem.h"

namespace Ui {
    class ADC_page;
}

class ADC_page : public QDialog
{
    Q_OBJECT

public:
    explicit ADC_page(QWidget *parent = 0);
    ~ADC_page();
    void update_data(int value);

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ADC_page *ui;
    QWidget* the_parent;
    QGraphicsScene *last_scene;
    GraphItem *item;

};

#endif // ADC_PAGE_H
