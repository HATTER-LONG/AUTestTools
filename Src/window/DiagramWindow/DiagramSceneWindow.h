#ifndef __DIAGRAM_SCENE_WINDOW_H__
#define __DIAGRAM_SCENE_WINDOW_H__

#include "DiagramItem.h"
#include "function/AnalysisMessage.h"

#include <QMainWindow>

class DiagramScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QToolBar;
QT_END_NAMESPACE

class DiagramSceneWindow : public QMainWindow
{
    Q_OBJECT
public:
    DiagramSceneWindow(QMainWindow* Parent);

    void drawResultByCodeMessage(const MyFunction::SourceCodeFunctionMessageMap& FunctionMessage,
        const MyFunction::SourceCodeErrorMessageList& ErrorMessage) const;
private slots:

    void backgroundButtonGroupClicked(QAbstractButton* Button);
    void buttonGroupClicked(int Id);
    void deleteItem();
    void pointerGroupClicked(int Id);
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem* Item);
    void textInserted(QGraphicsTextItem* Item);
    void currentFontChanged(const QFont& Font);
    void fontSizeChanged(const QString& Size);
    void sceneScaleChanged(const QString& Scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem* Item);
    void about();
    void itemSelectedToCreateSourceCode();

signals:
    void selectedFunctionInfo(std::string FuncName);

private:
    void createActions();
    void createToolBox();
    void createToolbars();
    void createItemMenus();
    QWidget* createBackgroundCellWidget(const QString& Text, const QString& Image);
    QWidget* createCellWidget(const QString& Text, DiagramItem::DiagramType Type);
    QMenu* createColorMenu(const char* Slot, QColor DefaultColor);

    QIcon createColorToolButtonIcon(const QString& Image, QColor Color);
    QIcon createColorIcon(QColor Color);

public:
    QAction* AddAction;
    QAction* DeleteAction;

    QAction* ToFrontAction;
    QAction* SendBackAction;
    QAction* AboutAction;

    QAction* SelectFuncAction;

private:
    // TODO: 整理多余的空间,删除代码逻辑
    DiagramScene* MyDiagramScene;
    QGraphicsView* MyView;

    QButtonGroup* ButtonGroup;
    QButtonGroup* PointerTypeGroup;
    QButtonGroup* BackgroundButtonGroup;

    QToolButton* FontColorToolButton;
    QToolButton* FillColorToolButton;
    QToolButton* LineColorToolButton;

    QComboBox* SceneScaleCombo;
    QComboBox* ItemColorCombo;
    QComboBox* TextColorCombo;
    QComboBox* FontSizeCombo;
    QFontComboBox* FontCombo;

    QToolBar* TextToolBar;
    QToolBar* EditToolBar;
    QToolBar* ColorToolBar;
    QToolBar* PointerToolbar;

    QMenu* FileMenu;
    QMenu* ItemMenu;
    QMenu* AboutMenu;

    QToolBox* ToolBox;

    QAction* BoldAction;
    QAction* UnderlineAction;
    QAction* ItalicAction;
    QAction* TextAction;
    QAction* FillAction;
    QAction* LineAction;
};

#endif