
#include "DiagramWindow/DiagramSceneWindow.h"
#include "ProduceWithEditWindow/ProduceWithEditWindow.h"
#include "mainwindow.h"
#include "spdlog/spdlog.h"

#include <QtWidgets>
#include <qaction.h>
#include <qboxlayout.h>
#include <qfiledialog.h>

MainWindow::MainWindow()
        : SourceCodeMessagePtr(nullptr)
{
    DiagramSceneWindowMember = new DiagramSceneWindow(this);
    createMenus();
    SourceCodeMessagePtr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass("level_1");
    EditWindow = new ProduceWithEditWindow(FunctionMessageMap, this);
    connect(DiagramSceneWindowMember, SIGNAL(selectedFunctionInfo(std::string)), EditWindow,
        SLOT(createSelectFuncTestCode(std::string)));
    auto* layout = new QHBoxLayout;
    auto* widgets = new QWidget;
    setCentralWidget(widgets);

    layout->addWidget(DiagramSceneWindowMember);
    layout->addWidget(EditWindow);
    layout->setStretch(0, 1);
    layout->setStretch(1, 1);
    widgets->setLayout(layout);
    setWindowTitle(tr("Diagramscene"));
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::createMenus()
{
    FileMenu = menuBar()->addMenu(tr("&File"));
    auto* openFileAction = new QAction(tr("&Open Source File"), this);
    openFileAction->setShortcut(tr("Ctrl+P"));
    openFileAction->setStatusTip(tr("Open source code file to analysis"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFileToAnalysis()));

    auto* setCompileFileAction = new QAction(tr("&Set Compile DataBase File"), this);
    setCompileFileAction->setStatusTip(tr("Open source code file to analysis"));
    connect(setCompileFileAction, SIGNAL(triggered()), this, SLOT(setCompileDateBaseFile()));
    auto* exitAction = new QAction(QIcon(":/images/exit.png"), tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    FileMenu->addAction(openFileAction);
    FileMenu->addAction(setCompileFileAction);
    FileMenu->addAction(exitAction);

    ItemMenu = menuBar()->addMenu(tr("&Item"));
    ItemMenu->addAction(DiagramSceneWindowMember->ToFrontAction);
    ItemMenu->addSeparator();
    ItemMenu->addAction(DiagramSceneWindowMember->SendBackAction);

    AboutInfoMenu = menuBar()->addMenu(tr("&Help"));
    AboutInfoMenu->addAction(DiagramSceneWindowMember->AboutAction);
}

void MainWindow::setCompileDateBaseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open compile database file"), "./", tr("json files (*.json)"));

    if (!fileName.isEmpty())
    {
        spdlog::info("{} filename is {}\n", __FUNCTION__, fileName.toStdString().c_str());
        SourceCodeMessagePtr->setCompileDatabase(fileName.toStdString());
    }
}

void MainWindow::openFileToAnalysis()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open source code file"), "./", tr("All files (*.*)"));

    if (!fileName.isEmpty())
    {
        spdlog::info("{} filename is {}\n", __FUNCTION__, fileName.toStdString().c_str());

        SourceCodeMessagePtr->setFilePathToAnalysis(fileName.toStdString());

        try
        {
            int result = SourceCodeMessagePtr->startToAnalysisSourceCode(FunctionMessageMap, ErrorMessageList);
            spdlog::info("StartToAnalysis Result is {}\n", result);
        }
        catch (std::exception& e)
        {
            spdlog::info("Start to analysis error exception: {}", e.what());
        }

        for (const auto& a : ErrorMessageList)
        {
            spdlog::info("errorLevel[{}] && message[{}] && filepos[{}]", a.getErrorLevel(), a.getErrorMessage(), a.getErrorPos());
        }

        DiagramSceneWindowMember->drawResultByCodeMessage(FunctionMessageMap, ErrorMessageList);
    }
}