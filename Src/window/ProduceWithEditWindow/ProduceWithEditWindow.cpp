#include "ProduceWithEditWindow.h"
#include "spdlog/spdlog.h"
#include <QtWidgets>

ProduceWithEditWindow::ProduceWithEditWindow(const MFunction::SourceCodeFunctionMessageMap& functionInfo,
                                             QWidget* parent)
        : QWidget(parent), functionMessage(functionInfo)
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

void ProduceWithEditWindow::createSelectFuncTestCode(std::string funcname)
{
    editor->append(QString(funcname.c_str()));
    auto function = (functionMessage.find(funcname))->second;
    spdlog::info("functionMessage[{}] ", function.GetFunctionName().c_str());

    auto paramlist = function.GetFunctionParam();

    for (const auto& a : paramlist)
    {
        spdlog::info("call function param [{}]", a.c_str());
    }
    auto callexprlist = function.GetFunctionWhichCallExpr();
    for (const auto& a : callexprlist)
    {
        spdlog::info("call function [{}]", a.c_str());
    }
}