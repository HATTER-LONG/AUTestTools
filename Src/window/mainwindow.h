#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "function/AnalysisMessage.h"
#include "function/SourceCodeAnalysisFunc.h"
#include <QMainWindow>
class DiagramSceneWindow;
class ProduceWithEditWindow;
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

    std::unique_ptr<MyFunction::SourceCodeAnalysisFunc> sourceCodeMessagePtr;
    MyFunction::SourceCodeFunctionMessageMap functionMessageMap;
    MyFunction::SourceCodeErrorMessageList errorMessageList;
};
#endif // MAINWINDOW_H
