#include "Arrow.h"
#include "DiagramItem.h"
#include "DiagramTextItem.h"
#include "spdlog/spdlog.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QTextBlockFormat>
#include <QTextCursor>

DiagramItem::DiagramItem(DiagramType DiagramType, QMenu* ContextMenu, QGraphicsItem* Parent)
        : QGraphicsPolygonItem(Parent)
{
    MyDiagramType = DiagramType;
    MyContextMenu = ContextMenu;

    QPainterPath path;
    switch (MyDiagramType)
    {
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            MyPolygon = path.toFillPolygon();
            break;
        case Conditional:
            MyPolygon << QPointF(-100, 0) << QPointF(0, 100) << QPointF(100, 0) << QPointF(0, -100) << QPointF(-100, 0);
            break;
        case Step:
            MyPolygon << QPointF(-100, -100) << QPointF(100, -100) << QPointF(100, 100) << QPointF(-100, 100)
                      << QPointF(-100, -100);
            break;
        default:
            MyPolygon << QPointF(-120, -80) << QPointF(-70, 80) << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(MyPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    createTextItem();
}

DiagramItem::~DiagramItem()
{
    delete MyTestItem;
}

void DiagramItem::removeArrow(Arrow* Arrow)
{
    int index = ArrowList.indexOf(Arrow);

    if (index != -1)
        ArrowList.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach (Arrow* arrow, ArrowList)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow* Arrow)
{
    ArrowList.append(Arrow);
}

void DiagramItem::setFont(const QFont& Font)
{
    MyFont = Font;
    MyTestItem->setFont(MyFont);
}

void DiagramItem::setTextColor(const QColor& Color)
{
    MyTestColor = Color;
    MyTestItem->setDefaultTextColor(MyTestColor);
}

void DiagramItem::createTextItem()
{
    MyTestItem = new DiagramTextItem(this);
    MyTestItem->setTextInteractionFlags(Qt::NoTextInteraction);
    MyTestItem->setTextWidth(this->boundingRect().width());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignHCenter);
    QTextCursor cursor = MyTestItem->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    MyTestItem->setTextCursor(cursor);
    MyTestItem->setPos(static_cast<qreal>(0 - (this->boundingRect().width() / 2)),
        static_cast<qreal>(0 - (boundingRect().height() / 4)));
    MyTestItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    MyTestItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
}
void DiagramItem::setItemText(const QString& Text)
{
    MyTestItem->setPlainText(Text);
}
const QString DiagramItem::getItemText()
{
    return MyTestItem->toPlainText();
}
QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(MyPolygon);

    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* Event)
{
    spdlog::info("{}:{} call!!", __FILE__, __FUNCTION__);
    scene()->clearSelection();
    setSelected(true);
    MyContextMenu->exec(Event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange Change, const QVariant& Value)
{
    if (Change == QGraphicsItem::ItemPositionChange)
    {
        foreach (Arrow* arrow, ArrowList)
        {
            arrow->updatePosition();
        }
    }

    return Value;
}

void DiagramItem::focusOutEvent(QFocusEvent* Event)
{
    if (Q_NULLPTR == MyTestItem)
    {
        QGraphicsPolygonItem::focusOutEvent(Event);
        return;
    }
    MyTestItem->setTextInteractionFlags(Qt::NoTextInteraction);
    MyTestItem->lostFocus(MyTestItem);

    QGraphicsPolygonItem::focusOutEvent(Event);
}

void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* Event)
{
    if (Q_NULLPTR == MyTestItem)
    {
        QGraphicsPolygonItem::mouseDoubleClickEvent(Event);
        return;
    }
    if (MyTestItem->textInteractionFlags() == Qt::NoTextInteraction)
    {
        MyTestItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        MyTestItem->setFocus();
    }

    QGraphicsPolygonItem::mouseDoubleClickEvent(Event);
}