#ifndef __DIAGRAM_SCENE_WINDOW_H__
#define __DIAGRAM_SCENE_WINDOW_H__

#include "DiagramItem.h"
#include "function/FunctionDeclAnalysis.h"
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
    DiagramSceneWindow(QMainWindow* parent);

    void drawReslutByCodeMessage(const MFunction::SourceCodeFunctionMessageMap& functionMessage,
                                 const MFunction::SourceCodeErrorMessageList& errorMessage) const;
private slots:

    void backgroundButtonGroupClicked(QAbstractButton* button);
    void buttonGroupClicked(int id);
    void deleteItem();
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem* item);
    void textInserted(QGraphicsTextItem* item);
    void currentFontChanged(const QFont& font);
    void fontSizeChanged(const QString& size);
    void sceneScaleChanged(const QString& scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem* item);
    void about();
    void itemSelectedToCreateSourceCode();

signals:
    void selectedFunctionInfo(std::string funcName);

private:
    void createActions();
    void createToolBox();
    void createToolbars();
    void createItemMenus();
    QWidget* createBackgroundCellWidget(const QString& text, const QString& image);
    QWidget* createCellWidget(const QString& text, DiagramItem::DiagramType type);
    QMenu* createColorMenu(const char* slot, QColor defaultColor);

    QIcon createColorToolButtonIcon(const QString& image, QColor color);
    QIcon createColorIcon(QColor color);

public:
    QAction* addAction;
    QAction* deleteAction;

    QAction* toFrontAction;
    QAction* sendBackAction;
    QAction* aboutAction;

    QAction* selectFuncAction;

private:
    DiagramScene* scene;
    QGraphicsView* view;

    QButtonGroup* buttonGroup;
    QButtonGroup* pointerTypeGroup;
    QButtonGroup* backgroundButtonGroup;

    QToolButton* fontColorToolButton;
    QToolButton* fillColorToolButton;
    QToolButton* lineColorToolButton;

    QComboBox* sceneScaleCombo;
    QComboBox* itemColorCombo;
    QComboBox* textColorCombo;
    QComboBox* fontSizeCombo;
    QFontComboBox* fontCombo;

    QToolBar* textToolBar;
    QToolBar* editToolBar;
    QToolBar* colorToolBar;
    QToolBar* pointerToolbar;

    QMenu* fileMenu;
    QMenu* itemMenu;
    QMenu* aboutMenu;

    QToolBox* toolBox;

    QAction* boldAction;
    QAction* underlineAction;
    QAction* italicAction;
    QAction* textAction;
    QAction* fillAction;
    QAction* lineAction;
};

#endif