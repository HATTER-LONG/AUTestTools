
#include "Arrow.h"

#include <QPainter>
#include <QPen>
#include <qmath.h>

Arrow::Arrow(DiagramItem* StartItem, DiagramItem* EndItem, QGraphicsItem* Parent)
        : QGraphicsLineItem(Parent)
{
    MyStartItem = StartItem;
    MyEndItem = EndItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    MyColor = Qt::black;
    setPen(QPen(MyColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(),
        QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(ArrowHead);
    return path;
}

void Arrow::updatePosition()
{
    QLineF line(mapFromItem(MyStartItem, 0, 0), mapFromItem(MyEndItem, 0, 0));
    setLine(line);
}

void Arrow::paint(QPainter* Painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (MyStartItem->collidesWithItem(MyEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(MyColor);
    qreal arrowSize = 20;
    Painter->setPen(myPen);
    Painter->setBrush(MyColor);

    QLineF centerLine(MyStartItem->pos(), MyEndItem->pos());
    QPolygonF endPolygon = MyEndItem->polygon();
    QPointF p1 = endPolygon.first() + MyEndItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i)
    {
        p2 = endPolygon.at(i) + MyEndItem->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersects(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, MyStartItem->pos()));

    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 =
        line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

    ArrowHead.clear();
    ArrowHead << line().p1() << arrowP1 << arrowP2;

    Painter->drawLine(line());
    Painter->drawPolygon(ArrowHead);
    if (isSelected())
    {
        Painter->setPen(QPen(MyColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        Painter->drawLine(myLine);
        myLine.translate(0, -8.0);
        Painter->drawLine(myLine);
    }
}
