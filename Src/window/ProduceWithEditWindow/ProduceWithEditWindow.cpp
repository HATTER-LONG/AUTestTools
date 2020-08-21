#include "ProduceWithEditWindow.h"
#include <QtWidgets>

ProduceWithEditWindow::ProduceWithEditWindow(QWidget* parent) : QWidget(parent)
{
    nameSetLable = new QLabel(this);
    QFont ft("Microsoft YaHei", 10);
    nameSetLable->setText(tr("Test Name"));
    nameSetLable->setFont(ft);

    testNameEdit = new QLineEdit(this);
    testTageEdit = new QLineEdit(this);
    editor = new QTextEdit(this);
    layoutView = new QGridLayout;

    layoutView->addWidget(nameSetLable, 0, 0);
    layoutView->addWidget(testNameEdit, 0, 1);
    layoutView->addWidget(testTageEdit, 0, 2);
    layoutView->setColumnStretch(0, 1);
    layoutView->setColumnStretch(1, 2);
    layoutView->setColumnStretch(2, 2);
    auto* layoutViewWidget = new QWidget;
    layoutViewWidget->setLayout(layoutView);
    auto* widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(layoutViewWidget);
    widgetLayout->addWidget(editor);
    this->setLayout(widgetLayout);
}

void ProduceWithEditWindow::createSelectFuncTestCode(QString funcname) { editor->append(funcname); }