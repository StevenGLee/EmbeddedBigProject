#ifndef GRAPHITEM_H
#define GRAPHITEM_H
#include <QGraphicsItem>
#include <QList>

class GraphItem : public QGraphicsItem
{
public:
    GraphItem();
    QRectF boundingRect() const;
    QList<int> valueList;
    int maxCount;
    int maxValue;
    int minValue;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setColor(const QColor &color) { brushColor = color; }
    void addValue(int val);
private:
    QColor brushColor;

};
#endif // GRAPHITEM_H
