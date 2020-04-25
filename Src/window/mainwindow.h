#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DiagramWindow/DiagramSceneWindow.h"
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
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

private:
    void createMenus();
    DiagramSceneWindow* diagramSceneWindow;
    QMenu* fileMenu;
    QMenu* itemMenu;
    QMenu* aboutMenu;
};
#endif // MAINWINDOW_H
