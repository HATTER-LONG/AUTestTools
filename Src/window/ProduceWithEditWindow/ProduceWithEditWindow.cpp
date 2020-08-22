#include "ProduceWithEditWindow.h"
#include <QtWidgets>

ProduceWithEditWindow::ProduceWithEditWindow(QWidget* parent) : QWidget(parent)
{
    createEditWindowItem();
    layoutView = new QGridLayout;

    layoutView->addWidget(nameSetLable, 0, 0);
    layoutView->addWidget(testNameEdit, 0, 1);
    layoutView->addWidget(testTageEdit, 0, 2);
    layoutView->addWidget(paramSetLable, 1, 0);
    layoutView->addWidget(paramEdit, 1, 1, 1, 2);
    layoutView->addWidget(sectionNameSetLabel, 2, 0);
    layoutView->addWidget(sectionSetEdit, 2, 1);
    layoutView->addWidget(sectionCheckEdit, 2, 2);
    layoutView->addWidget(buttonCreateTestCode, 3, 1);
    layoutView->addWidget(buttonCreateMock, 3, 2);
    layoutView->setColumnStretch(0, 1);
    layoutView->setColumnStretch(1, 2);
    layoutView->setColumnStretch(2, 2);
    auto* layoutViewWidget = new QWidget;
    layoutViewWidget->setLayout(layoutView);

    auto* widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(layoutViewWidget);

    editor = new QTextEdit;
    widgetLayout->addWidget(editor);
    this->setLayout(widgetLayout);
}

void ProduceWithEditWindow::createEditWindowItem()
{
    nameSetLable = new QLabel;
    QFont ft("Microsoft YaHei", 10);
    nameSetLable->setText(tr("Test Name"));
    nameSetLable->setFont(ft);
    testNameEdit = new QLineEdit;
    testTageEdit = new QLineEdit;

    paramSetLable = new QLabel;
    paramSetLable->setText(tr("Param"));
    paramSetLable->setFont(ft);
    paramEdit = new QLineEdit;

    sectionNameSetLabel = new QLabel;
    sectionNameSetLabel->setText(tr("Section & Check"));
    sectionNameSetLabel->setFont(ft);

    sectionSetEdit = new QLineEdit;
    sectionCheckEdit = new QLineEdit;

    buttonCreateTestCode = new QPushButton;
    buttonCreateTestCode->setText(tr("Create Code"));
    buttonCreateMock = new QPushButton;
    buttonCreateMock->setText(tr("Create Mock"));
}

void ProduceWithEditWindow::createSelectFuncTestCode(QString funcname) { editor->append(funcname); }