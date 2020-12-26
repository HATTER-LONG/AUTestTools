#ifndef ARROW_H
#define ARROW_H

#include "DiagramItem.h"

#include <QGraphicsLineItem>

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Arrow : public QGraphicsLineItem
{
public:
    enum
    {
        Type = UserType + 4
    };

    Arrow(DiagramItem* StartItem, DiagramItem* EndItem, QGraphicsItem* Parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor& Color) { MyColor = Color; }
    DiagramItem* startItem() const { return MyStartItem; }
    DiagramItem* endItem() const { return MyEndItem; }

    void updatePosition();

protected:
    void paint(QPainter* Painter, const QStyleOptionGraphicsItem* Option, QWidget* Widget = nullptr) override;

private:
    DiagramItem* MyStartItem;
    DiagramItem* MyEndItem;
    QColor MyColor;
    QPolygonF ArrowHead;
};

#endif   // ARROW_H
