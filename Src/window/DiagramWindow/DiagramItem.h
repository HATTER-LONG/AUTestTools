#ifndef __DIAGRAM_ITEM_H__
#define __DIAGRAM_ITEM_H__

#include "DiagramTextItem.h"
#include <QGraphicsPolygonItem>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;

class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum
    {
        Type = UserType + 15
    };

    enum DiagramType
    {
        Step,
        Conditional,
        StartEnd,
        Io
    };

    DiagramItem(DiagramType diagramType, QMenu* contextMenu, QGraphicsItem* parent = nullptr);
    ~DiagramItem() override;

    void removeArrow(Arrow* arrow);
    void removeArrows();
    DiagramType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(Arrow* arrow);
    QPixmap image() const;
    int type() const override { return Type; }
    void setFont(const QFont& font);
    void setTextColor(const QColor& color);
    void createTextItem();

    void setItemText(const QString& text);
    const QString getItemText();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void focusOutEvent(QFocusEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QMenu* myContextMenu;
    QList<Arrow*> arrows;
    DiagramTextItem* mytextItem;

    QColor myTextColor;
    QFont myFont;
};
#endif