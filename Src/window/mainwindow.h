#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DiagramSceneWindow;
class ProduceWithEditWindow;
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
    void setCompileDateBaseFile();

private:
    void createMenus();

    DiagramSceneWindow* diagramSceneWindow;
    ProduceWithEditWindow* editwindow;
    QMenu* fileMenu;
    QMenu* itemMenu;
    QMenu* aboutMenu;

    MFunction::FunctionDeclAnalysis* sourceCodeMessagePtr;
};
#endif // MAINWINDOW_H
