#include "DiagramScene.h"
#include "DiagramTextItem.h"
#include "spdlog/spdlog.h"

DiagramTextItem::DiagramTextItem(QGraphicsItem* Parent)
        : QGraphicsTextItem(Parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QVariant DiagramTextItem::itemChange(GraphicsItemChange Change, const QVariant& Value)
{
    if (Change == QGraphicsItem::ItemSelectedHasChanged) emit selectedChange(this);
    return Value;
}

void DiagramTextItem::focusOutEvent(QFocusEvent* Event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(Event);
}

void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* Event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction) setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(Event);
}