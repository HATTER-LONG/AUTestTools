#include "ProduceWithEditWindow.h"
#include "function/UnitTestCodeProduce.h"
#include "spdlog/spdlog.h"

#include <QtWidgets>
#include <string>

ProduceWithEditWindow::ProduceWithEditWindow(const MyFunction::SourceCodeFunctionMessageMap& FunctionInfo, QWidget* Parent)
        : QWidget(Parent)
        , FunctionMessageMap(FunctionInfo)
{
    createEditWindowItem();
    LayoutView = new QGridLayout;

    LayoutView->addWidget(InputNameLabel, 0, 0);
    LayoutView->addWidget(TestNameEdit, 0, 1);
    LayoutView->addWidget(TestTageEdit, 0, 2);
    LayoutView->addWidget(InputParamLabel, 1, 0);
    LayoutView->addWidget(ParamEdit, 1, 1, 1, 2);
    LayoutView->addWidget(InputSectionNamelabel, 2, 0);
    LayoutView->addWidget(SectionSetEdit, 2, 1);
    LayoutView->addWidget(SectionCheckEdit, 2, 2);
    LayoutView->addWidget(CreateTestCodeButton, 3, 1);
    LayoutView->addWidget(CreateMockCodeButton, 3, 2);
    LayoutView->setColumnStretch(0, 1);
    LayoutView->setColumnStretch(1, 2);
    LayoutView->setColumnStretch(2, 2);
    auto* layoutViewWidget = new QWidget;
    layoutViewWidget->setLayout(LayoutView);

    auto* widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(layoutViewWidget);

    Editor = new QTextEdit;
    widgetLayout->addWidget(Editor);
    this->setLayout(widgetLayout);
}

void ProduceWithEditWindow::createEditWindowItem()
{
    InputNameLabel = new QLabel;
    QFont ft("Microsoft YaHei", 10);
    InputNameLabel->setText(tr("Test Name"));
    InputNameLabel->setFont(ft);
    TestNameEdit = new QLineEdit;
    TestTageEdit = new QLineEdit;

    InputParamLabel = new QLabel;
    InputParamLabel->setText(tr("Param"));
    InputParamLabel->setFont(ft);
    ParamEdit = new QLineEdit;

    InputSectionNamelabel = new QLabel;
    InputSectionNamelabel->setText(tr("Section & Check"));
    InputSectionNamelabel->setFont(ft);

    SectionSetEdit = new QLineEdit;
    SectionCheckEdit = new QLineEdit;

    CreateTestCodeButton = new QPushButton;
    CreateTestCodeButton->setText(tr("Create Code"));
    connect(CreateTestCodeButton, SIGNAL(clicked()), this, SLOT(createUnitTestCodeButtonClicked()));
    CreateMockCodeButton = new QPushButton;
    CreateMockCodeButton->setText(tr("Create Mock"));
    connect(CreateMockCodeButton, SIGNAL(clicked()), this, SLOT(createMockCodeButtonClicked()));
}

void ProduceWithEditWindow::createSelectFuncTestCode(std::string Funcname)
{
    FuncNameHasSelected = Funcname;
    auto functioniter = FunctionMessageMap.find(Funcname);

    if (functioniter != FunctionMessageMap.end())
    {
        spdlog::info("functionMessage[{}] ", functioniter->second.getFunctionName().c_str());

        MyFunction::UnitTestCodeProduceFunc produceCode;
        std::string functionMessage = produceCode.createMockSourceCode(functioniter->second);
        Editor->setText(QString(functionMessage.c_str()));
    }
    else
    {
        spdlog::info("{} can't find selected function[{}] Info", __FUNCTION__, FuncNameHasSelected.c_str());
    }
}

void ProduceWithEditWindow::createMockCodeButtonClicked()
{
    MyFunction::UnitTestCodeProduceFunc produceCode;

    std::string functionCode2MOCK = produceCode.createMockSourceCode(FunctionMessageMap);

    Editor->setText(QString(functionCode2MOCK.c_str()));
}

void ProduceWithEditWindow::createUnitTestCodeButtonClicked()
{
    MyFunction::UnitTestCodeProduceFunc produceCode;

    MyFunction::UnitTestSectionInfo unitTestSectionInfo;
    unitTestSectionInfo.m_sectionName = SectionSetEdit->text().toStdString();
    unitTestSectionInfo.m_checkInfo = SectionCheckEdit->text().toStdString();

    MyFunction::UnitTestInfo unitTestinfo;
    unitTestinfo.m_testSection.emplace_back(unitTestSectionInfo);
    unitTestinfo.m_testName = TestNameEdit->text().toStdString();
    getTagName(unitTestinfo.m_testTags);

    auto iter = FunctionMessageMap.find(FuncNameHasSelected);
    if (iter != FunctionMessageMap.end())
    {
        std::string unitTestCode = produceCode.createUnitTestCode(iter->second, unitTestinfo);
        Editor->setText(QString(unitTestCode.c_str()));
    }
    else
    {
        spdlog::info("{} can't find selected function[{}] Info", __FUNCTION__, FuncNameHasSelected.c_str());
    }
}

void ProduceWithEditWindow::getTagName(std::vector<std::string>& TagBox)
{
    std::string tagTextSplitWithBlank = TestTageEdit->text().toStdString();
    std::string tmptag;
    std::string::size_type posSubstringStart;
    std::string::size_type posSeparator;

    posSeparator = tagTextSplitWithBlank.find(' ');
    posSubstringStart = 0;
    while (std::string::npos != posSeparator)
    {
        TagBox.emplace_back(tagTextSplitWithBlank.substr(0, posSeparator));
        tagTextSplitWithBlank = tagTextSplitWithBlank.substr(posSeparator + 1, tagTextSplitWithBlank.length());
        posSeparator = tagTextSplitWithBlank.find(' ');
    }
    TagBox.emplace_back(tagTextSplitWithBlank);
}