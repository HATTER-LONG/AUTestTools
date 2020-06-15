#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DiagramWindow/DiagramSceneWindow.h"
#include <QMainWindow>

class DiagramScene;

namespace MFunction
{
class FunctionDeclAnalysis;
} // namespace MFunction

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

private slots:
    void openFileToAnalysis();

private:
    void createMenus();

    DiagramSceneWindow* diagramSceneWindow;
    QMenu* fileMenu;
    QMenu* itemMenu;
    QMenu* aboutMenu;

    MFunction::FunctionDeclAnalysis* sourceCodeMessagePtr;
};
#endif // MAINWINDOW_H
