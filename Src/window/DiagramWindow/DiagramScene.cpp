#include "Arrow.h"
#include "DiagramScene.h"
#include "spdlog/spdlog.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>

DiagramScene::DiagramScene(QMenu* ItemMenu, QObject* Parent)
        : QGraphicsScene(Parent)
{
    MyItemMenu = ItemMenu;
    MyMode = MoveItem;
    MyItemType = DiagramItem::Step;
    Line = nullptr;
    TextItem = nullptr;
    MyItemColor = Qt::white;
    MyTextColor = Qt::black;
    MyLineColor = Qt::black;
}

bool DiagramScene::isItemChange(int Type)
{
    foreach (QGraphicsItem* item, selectedItems())
    {
        if (item->type() == Type) return true;
    }
    return false;
}

void DiagramScene::setLineColor(const QColor& Color)
{
    MyLineColor = Color;
    if (isItemChange(Arrow::Type))
    {
        auto* item = qgraphicsitem_cast<Arrow*>(selectedItems().first());
        item->setColor(MyLineColor);
        update();
    }
}

void DiagramScene::setTextColor(const QColor& Color)
{
    MyTextColor = Color;
    if (isItemChange(DiagramTextItem::Type))
    {
        auto* item =
            qgraphicsitem_cast<DiagramTextItem*>(selectedItems().first());
        item->setDefaultTextColor(MyTextColor);
    }
}

void DiagramScene::setItemColor(const QColor& Color)
{
    MyItemColor = Color;
    if (isItemChange(DiagramItem::Type))
    {
        auto* item = qgraphicsitem_cast<DiagramItem*>(selectedItems().first());
        item->setBrush(MyItemColor);
    }
}

void DiagramScene::setFont(const QFont& Font)
{
    MyFont = Font;
    if (isItemChange(DiagramTextItem::Type))
    {
        QGraphicsTextItem* item =
            qgraphicsitem_cast<DiagramTextItem*>(selectedItems().first());
        // At this point the selection can change so the first selected item
        // might not be a DiagramTextItem
        if (item) item->setFont(MyFont);
    }
}

void DiagramScene::setMode(Mode Mode)
{
    MyMode = Mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType Type)
{
    MyItemType = Type;
}

void DiagramScene::editorLostFocus(DiagramTextItem* Item)
{
    QTextCursor cursor = Item->textCursor();
    cursor.clearSelection();
    Item->setTextCursor(cursor);

    if (Item->toPlainText().isEmpty())
    {
        removeItem(Item);
        Item->deleteLater();
    }
}
DiagramItem* DiagramScene::createItem(DiagramItem::DiagramType Type,
    QPointF Point)
{
    auto* item = new DiagramItem(Type, MyItemMenu);
    item->setBrush(MyItemColor);
    item->setFont(MyFont);
    item->setTextColor(MyTextColor);
    item->setPos(Point);
    addItem(item);
    return item;
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent* MouseEvent)
{
    if (MouseEvent->button() != Qt::LeftButton) return;

    switch (MyMode)
    {
        case InsertItem:
            emit itemInserted(createItem(MyItemType, MouseEvent->scenePos()));
            break;
        case InsertLine:
            Line = new QGraphicsLineItem(
                QLineF(MouseEvent->scenePos(), MouseEvent->scenePos()));
            Line->setPen(QPen(MyLineColor, 2));
            addItem(Line);
            break;
        case InsertText:
            TextItem = new DiagramTextItem();
            TextItem->setFont(MyFont);
            TextItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            TextItem->setZValue(1000.0);
            connect(TextItem, SIGNAL(lostFocus(DiagramTextItem*)), this,
                SLOT(editorLostFocus(DiagramTextItem*)));
            connect(TextItem, SIGNAL(selectedChange(QGraphicsItem*)), this,
                SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(TextItem);
            TextItem->setDefaultTextColor(MyTextColor);
            TextItem->setPos(MouseEvent->scenePos());
            emit textInserted(TextItem);
        default:;
    }
    QGraphicsScene::mousePressEvent(MouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* MouseEvent)
{
    if (MyMode == InsertLine && Line != nullptr)
    {
        QLineF newLine(Line->line().p1(), MouseEvent->scenePos());
        Line->setLine(newLine);
    }
    else if (MyMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(MouseEvent);
    }
}

void DiagramScene::setArrow(DiagramItem* StartItem, DiagramItem* EndItem)
{
    auto* arrow = new Arrow(StartItem, EndItem);
    arrow->setColor(MyLineColor);
    StartItem->addArrow(arrow);
    EndItem->addArrow(arrow);
    arrow->setZValue(-1000.0);
    addItem(arrow);
    arrow->updatePosition();
}

bool DiagramScene::isDiagramItem(QGraphicsItem* Item)
{
    return Item->type() == DiagramItem::Type ||
           (Item->parentItem() != nullptr &&
               Item->parentItem()->type() == DiagramItem::Type);
}
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* MouseEvent)
{
    if (Line != nullptr && MyMode == InsertLine)
    {
        QList<QGraphicsItem*> startItems = items(Line->line().p1());
        if (startItems.count() && startItems.first() == Line)
            startItems.removeFirst();
        QList<QGraphicsItem*> endItems = items(Line->line().p2());
        if (endItems.count() && endItems.first() == Line)
            endItems.removeFirst();

        removeItem(Line);
        delete Line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first())
        {
            auto* startItem = startItems.first();
            auto* endItem = endItems.first();

            if (isDiagramItem(startItem) && isDiagramItem(endItem))
            {
                if (startItem->type() != DiagramItem::Type)
                {
                    startItem = startItem->parentItem();
                }
                if (endItem->type() != DiagramItem::Type)
                {
                    endItem = endItem->parentItem();
                }
                setArrow(qgraphicsitem_cast<DiagramItem*>(startItem),
                    qgraphicsitem_cast<DiagramItem*>(endItem));
            }
            else
            {
                spdlog::info("[{}] can not to arrow this Item\n", __FUNCTION__);
            }
        }
    }

    Line = nullptr;
    QGraphicsScene::mouseReleaseEvent(MouseEvent);
}