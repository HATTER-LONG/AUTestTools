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

DiagramItem::DiagramItem(DiagramType diagramType, QGraphicsItem* parent) : QGraphicsPolygonItem(parent)
{
    spdlog::info("DiagramItem Build  diagramType = [{}]\n", diagramType);
    myDiagramType = diagramType;
    // myContextMenu = contextMenu;

    QPainterPath path;
    switch (myDiagramType)
    {
    case StartEnd:
        path.moveTo(200, 50);
        path.arcTo(150, 0, 50, 50, 0, 90);
        path.arcTo(50, 0, 50, 50, 90, 90);
        path.arcTo(50, 50, 50, 50, 180, 90);
        path.arcTo(150, 50, 50, 50, 270, 90);
        path.lineTo(200, 25);
        myPolygon = path.toFillPolygon();
        break;
    case Conditional:
        myPolygon << QPointF(-100, 0) << QPointF(0, 100) << QPointF(100, 0) << QPointF(0, -100) << QPointF(-100, 0);
        break;
    case Step:
        myPolygon << QPointF(-100, -100) << QPointF(100, -100) << QPointF(100, 100) << QPointF(-100, 100)
                  << QPointF(-100, -100);
        break;
    default:
        myPolygon << QPointF(-120, -80) << QPointF(-70, 80) << QPointF(120, 80) << QPointF(70, -80)
                  << QPointF(-120, -80);
        break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    createTextItem();
}

DiagramItem::~DiagramItem() { delete mytextItem; }

void DiagramItem::removeArrow(Arrow* arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach (Arrow* arrow, arrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow* arrow) { arrows.append(arrow); }

void DiagramItem::setFont(const QFont& font)
{
    myFont = font;
    mytextItem->setFont(myFont);
}

void DiagramItem::setTextColor(const QColor& color)
{
    myTextColor = color;
    mytextItem->setDefaultTextColor(myTextColor);
}

void DiagramItem::createTextItem()
{
    mytextItem = new DiagramTextItem(this);
    mytextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    mytextItem->setTextWidth(this->boundingRect().width());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignHCenter);
    QTextCursor cursor = mytextItem->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    mytextItem->setTextCursor(cursor);
    mytextItem->setPos(static_cast<qreal>(0 - (this->boundingRect().width() / 2)),
                       static_cast<qreal>(0 - (boundingRect().height() / 4)));
    mytextItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    mytextItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
}
void DiagramItem::setItemText(const QString& text) { mytextItem->setPlainText(text); }

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* /*event*/)
{
    spdlog::info("{}:{} call!!", __FILE__, __FUNCTION__);
    scene()->clearSelection();
    setSelected(true);
    // myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach (Arrow* arrow, arrows)
        {
            arrow->updatePosition();
        }
    }

    return value;
}

void DiagramItem::focusOutEvent(QFocusEvent* event)
{
    if (Q_NULLPTR == mytextItem)
    {
        QGraphicsPolygonItem::focusOutEvent(event);
        return;
    }
    mytextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    mytextItem->lostFocus(mytextItem);

    QGraphicsPolygonItem::focusOutEvent(event);
}

void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (Q_NULLPTR == mytextItem)
    {
        QGraphicsPolygonItem::mouseDoubleClickEvent(event);
        return;
    }
    if (mytextItem->textInteractionFlags() == Qt::NoTextInteraction)
    {
        mytextItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        mytextItem->setFocus();
    }

    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
}