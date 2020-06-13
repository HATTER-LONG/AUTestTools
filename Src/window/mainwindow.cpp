
#include "mainwindow.h"
#include "spdlog/spdlog.h"

#include <QtWidgets>

MainWindow::MainWindow()
{
    diagramSceneWindow = new DiagramSceneWindow(this);
    createMenus();
    setCentralWidget(diagramSceneWindow);
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
        diagramSceneWindow->fileopen(fileName);
    }
}