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

    DiagramSceneWindow* DiagramSceneWindowMember;
    ProduceWithEditWindow* EditWindow;
    QMenu* FileMenu;
    QMenu* ItemMenu;
    QMenu* AboutInfoMenu;

    std::unique_ptr<MyFunction::SourceCodeAnalysisFunc> SourceCodeMessagePtr;
    MyFunction::SourceCodeFunctionMessageMap FunctionMessageMap;
    MyFunction::SourceCodeErrorMessageList ErrorMessageList;
};
#endif   // MAINWINDOW_H
