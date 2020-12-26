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

    DiagramItem(DiagramType DiagramType, QMenu* ContextMenu, QGraphicsItem* Parent = nullptr);
    ~DiagramItem() override;

    void removeArrow(Arrow* Arrow);
    void removeArrows();
    DiagramType diagramType() const { return MyDiagramType; }
    QPolygonF polygon() const { return MyPolygon; }
    void addArrow(Arrow* Arrow);
    QPixmap image() const;
    int type() const override { return Type; }
    void setFont(const QFont& Font);
    void setTextColor(const QColor& Color);
    void createTextItem();

    void setItemText(const QString& Text);
    const QString getItemText();

    void setFunctionName(std::string FunctionName) { MyFunctionName = FunctionName; }
    std::string getFunctionName() const { return MyFunctionName; }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* Event) override;
    QVariant itemChange(GraphicsItemChange Change,
        const QVariant& Value) override;
    void focusOutEvent(QFocusEvent* Event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* Event) override;

private:
    DiagramType MyDiagramType;
    QPolygonF MyPolygon;
    QMenu* MyContextMenu;
    QList<Arrow*> ArrowList;
    DiagramTextItem* MyTestItem;

    QColor MyTestColor;
    QFont MyFont;

    std::string MyFunctionName;
};
#endif