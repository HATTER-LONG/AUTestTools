
#include "function/FunctionDeclAnalysis.h"
#include "mainwindow.h"
#include "spdlog/spdlog.h"
#include <QtWidgets>
#include <qboxlayout.h>

MainWindow::MainWindow() : sourceCodeMessagePtr(nullptr)
{
    diagramSceneWindow = new DiagramSceneWindow(this);
    createMenus();
    sourceCodeMessagePtr = new MFunction::FunctionDeclAnalysis("", "./build/compile_commands.json");
    editwindow = new ProduceWithEditWindow(sourceCodeMessagePtr->GetFunctionMessageRef(), this);
    connect(diagramSceneWindow, SIGNAL(selectedFunctionInfo(std::string)), editwindow,
            SLOT(createSelectFuncTestCode(std::string)));
    auto* layout = new QHBoxLayout;
    auto* widgets = new QWidget;
    setCentralWidget(widgets);

    layout->addWidget(diagramSceneWindow);
    layout->addWidget(editwindow);
    layout->setStretch(0, 1);
    layout->setStretch(1, 1);
    widgets->setLayout(layout);
    setWindowTitle(tr("Diagramscene"));
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    auto openFileAction = new QAction(tr("&Open"), this);
    openFileAction->setShortcut(tr("Ctrl+P"));
    openFileAction->setStatusTip(tr("Open source code file to analysis"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFileToAnalysis()));

    auto exitAction = new QAction(QIcon(":/images/exit.png"), tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(diagramSceneWindow->toFrontAction);
    itemMenu->addSeparator();
    itemMenu->addAction(diagramSceneWindow->sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(diagramSceneWindow->aboutAction);
}

void MainWindow::openFileToAnalysis()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open source code file"), "./", tr("All files (*.*)"));

    if (!fileName.isEmpty())
    {
        spdlog::info("{} filename is {}\n", __FUNCTION__, fileName.toStdString().c_str());

        sourceCodeMessagePtr->SetFilePathToAnalysis(fileName.toStdString());

        try
        {
            int result = sourceCodeMessagePtr->StartToAnalysis();
            spdlog::info("StartToAnalysis Result is {}\n", result);
        }
        catch (std::exception& e)
        {
            spdlog::info("Start to analysis error exception: {}", e.what());
        }

        const MFunction::SourceCodeErrorMessageList& tmpErrorMessagevector = sourceCodeMessagePtr->GetErrorMessageRef();
        for (auto a : tmpErrorMessagevector)
        {
            spdlog::info("errorLevel[{}] && message[{}] && filepos[{}]", a.GetErrorLevel(), a.GetErrorMessage(),
                         a.GetErrorPos());
        }

        diagramSceneWindow->drawResultByCodeMessage(sourceCodeMessagePtr->GetFunctionMessageRef(),
                                                    tmpErrorMessagevector);
    }
}