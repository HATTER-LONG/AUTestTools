
#include "mainwindow.h"
#include "DiagramItem.h"
#include "DiagramScene.h"
#include "spdlog/spdlog.h"

#include <QtWidgets>

const int InsertTextButton = 10;
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
    fileMenu->addAction("exit", this, SLOT(close()));

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(diagramSceneWindow->toFrontAction);
    itemMenu->addSeparator();
    itemMenu->addAction(diagramSceneWindow->toFrontAction);
    itemMenu->addAction(diagramSceneWindow->sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(diagramSceneWindow->aboutAction);
}