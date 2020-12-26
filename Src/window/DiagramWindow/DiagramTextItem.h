#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum
    {
        Type = UserType + 3
    };

    DiagramTextItem(QGraphicsItem* Parent = nullptr);
    int type() const override { return Type; }

signals:
    void lostFocus(DiagramTextItem* Item);
    void selectedChange(QGraphicsItem* Item);

protected:
    QVariant itemChange(GraphicsItemChange Change, const QVariant& Value) override;
    void focusOutEvent(QFocusEvent* Event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* Event) override;
};

#endif   // DIAGRAMTEXTITEM_H
