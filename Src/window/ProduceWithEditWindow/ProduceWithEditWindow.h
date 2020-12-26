#pragma once
#include "function/AnalysisMessage.h"

#include <QWidget>
#include <vector>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QGridLayout;
class QPushButton;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE


class ProduceWithEditWindow : public QWidget
{
    Q_OBJECT
public:
    ProduceWithEditWindow(const MyFunction::SourceCodeFunctionMessageMap& FunctionInfo,
        QWidget* Parent = nullptr);

public slots:
    void createSelectFuncTestCode(std::string Funcname);
    void createMockCodeButtonClicked();
    void createUnitTestCodeButtonClicked();

private:
    void createEditWindowItem();
    void getTagName(std::vector<std::string>&);

private:
    QGridLayout* LayoutView;
    QTextEdit* Editor;
    QLabel* InputNameLabel;
    QLabel* InputParamLabel;
    QLabel* InputSectionNamelabel;

    QLineEdit* TestNameEdit;
    QLineEdit* TestTageEdit;
    QLineEdit* ParamEdit;
    QLineEdit* SectionSetEdit;
    QLineEdit* SectionCheckEdit;

    QPushButton* CreateTestCodeButton;
    QPushButton* CreateMockCodeButton;

    const MyFunction::SourceCodeFunctionMessageMap& FunctionMessageMap;
    std::string FuncNameHasSelected;
};