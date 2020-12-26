#ifndef __DIAGRAM_SCENE_H__
#define __DIAGRAM_SCENE_H__

#include "DiagramItem.h"
#include "DiagramTextItem.h"

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode
    {
        InsertItem,
        InsertLine,
        InsertText,
        MoveItem
    };

    explicit DiagramScene(QMenu* ItemMenu, QObject* Parent = nullptr);
    QFont font() const { return MyFont; }
    QColor textColor() const { return MyTextColor; }
    QColor itemColor() const { return MyItemColor; }
    QColor lineColor() const { return MyLineColor; }
    void setLineColor(const QColor& Color);
    void setTextColor(const QColor& Color);
    void setItemColor(const QColor& Color);
    void setFont(const QFont& Font);
    DiagramItem* createItem(DiagramItem::DiagramType Type, QPointF Point);
    void setArrow(DiagramItem* StartItem, DiagramItem* EndItem);
public slots:
    void setMode(Mode Mode);
    void setItemType(DiagramItem::DiagramType Type);
    void editorLostFocus(DiagramTextItem* Item);

signals:
    void itemInserted(DiagramItem* Item);
    void textInserted(QGraphicsTextItem* Item);
    void itemSelected(QGraphicsItem* Item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* MouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* MouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* MouseEvent) override;

private:
    bool isItemChange(int Type);
    bool isDiagramItem(QGraphicsItem* Item);
    DiagramItem::DiagramType MyItemType;
    QMenu* MyItemMenu;
    Mode MyMode;
    bool LeftButtonDown;
    QPointF StartPoint;
    QGraphicsLineItem* Line;
    QFont MyFont;
    DiagramTextItem* TextItem;
    QColor MyTextColor;
    QColor MyItemColor;
    QColor MyLineColor;
};
#endif