#include "Arrow.h"
#include "DiagramScene.h"
#include "spdlog/spdlog.h"


#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>

DiagramScene::DiagramScene(QMenu* itemMenu, QObject* parent) : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = nullptr;
    textItem = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}

bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem* item, selectedItems())
    {
        if (item->type() == type)
            return true;
    }
    return false;
}

void DiagramScene::setLineColor(const QColor& color)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    myLineColor = color;
    if (isItemChange(Arrow::Type))
    {
        auto* item = qgraphicsitem_cast<Arrow*>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void DiagramScene::setTextColor(const QColor& color)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type))
    {
        auto* item = qgraphicsitem_cast<DiagramTextItem*>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void DiagramScene::setItemColor(const QColor& color)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    myItemColor = color;
    if (isItemChange(DiagramItem::Type))
    {
        auto* item = qgraphicsitem_cast<DiagramItem*>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

void DiagramScene::setFont(const QFont& font)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    myFont = font;
    if (isItemChange(DiagramTextItem::Type))
    {
        QGraphicsTextItem* item = qgraphicsitem_cast<DiagramTextItem*>(selectedItems().first());
        // At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}

void DiagramScene::setMode(Mode mode) { myMode = mode; }

void DiagramScene::setItemType(DiagramItem::DiagramType type) { myItemType = type; }

void DiagramScene::editorLostFocus(DiagramTextItem* item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty())
    {
        removeItem(item);
        item->deleteLater();
    }
}
DiagramItem* DiagramScene::createItem(DiagramItem::DiagramType type, QPointF point)
{
    auto* item = new DiagramItem(type, myItemMenu);
    item->setBrush(myItemColor);
    item->setFont(myFont);
    item->setTextColor(myTextColor);
    item->setPos(point);
    addItem(item);
    return item;
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    switch (myMode)
    {
    case InsertItem:
        emit itemInserted(createItem(myItemType, mouseEvent->scenePos()));
        break;
    case InsertLine:
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen(QPen(myLineColor, 2));
        addItem(line);
        break;
    case InsertText:
        textItem = new DiagramTextItem();
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)), this, SLOT(editorLostFocus(DiagramTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        textItem->setPos(mouseEvent->scenePos());
        emit textInserted(textItem);
    default:;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (myMode == InsertLine && line != nullptr)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void DiagramScene::setArrow(DiagramItem* startItem, DiagramItem* endItem)
{
    auto* arrow = new Arrow(startItem, endItem);
    arrow->setColor(myLineColor);
    startItem->addArrow(arrow);
    endItem->addArrow(arrow);
    arrow->setZValue(-1000.0);
    addItem(arrow);
    arrow->updatePosition();
}

bool DiagramScene::isDiagramItem(QGraphicsItem* item)
{
    return item->type() == DiagramItem::Type ||
           (item->parentItem() != nullptr && item->parentItem()->type() == DiagramItem::Type);
}
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (line != nullptr && myMode == InsertLine)
    {
        QList<QGraphicsItem*> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem*> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 && startItems.first() != endItems.first())
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
                setArrow(qgraphicsitem_cast<DiagramItem*>(startItem), qgraphicsitem_cast<DiagramItem*>(endItem));
            }
            else
            {
                spdlog::info("[{}] can not to arrow this Item\n", __FUNCTION__);
            }
        }
    }

    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}