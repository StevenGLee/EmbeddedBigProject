#include "GraphItem.h"
#include <QPainter>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
//#include <QDebug.h>
#include <QVector>
#include <stdio.h>

GraphItem::GraphItem()
{
    brushColor = Qt::red;

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);

}
void GraphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                   QWidget *)
{

    printf("call paint()");
    if (hasFocus()) {
        painter->setPen(QPen(QColor(255, 255, 255, 200)));
    } else {
        painter->setPen(QPen(QColor(100, 100, 100, 100)));
    }
    painter->setBrush(brushColor);
    //painter->drawRect(-10, -10, 20, 20);

    //QVector<QPointF> points;
    // 定义四个点
    //static const QPointF points[4] = {
    //QPointF(200.0, 80.0),
    //QPointF(220.0, 10.0),
    //QPointF(270.0, 30.0),
    //QPointF(310.0, 70.0)
    //};
    QPainterPath path;
    path.moveTo(0, (1-(double)(valueList[0])/(maxValue-minValue))*231);
    for(int i=1;i<valueList.count();i++)
    {
        path.lineTo(((double)i/(maxCount-1))*361,(1-(double)(valueList[1])/(maxValue-minValue))*231);
    }
    // 使用四个点绘制多边形
    //painter->drawPolygon(points, 4);
    painter -> drawPath(path);

}

void GraphItem::addValue(int val)
{
    valueList += val;
    if(valueList.count()>maxCount)
        valueList.removeFirst();
    printf("call addValue() success, valueList.count() returns %d",valueList.count());

}


QRectF GraphItem::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(0 - adjust, 0 - adjust,
                  361 + adjust, 231 + adjust);
}
