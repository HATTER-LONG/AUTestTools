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
    ProduceWithEditWindow(const MyFunction::SourceCodeFunctionMessageMap& functionInfo, QWidget* parent = nullptr);

public slots:
    void createSelectFuncTestCode(std::string funcname);
    void createMockCodeButtonClicked();
    void createUnitTestCodeButtonClicked();

private:
    void createEditWindowItem();
    void getTagName(std::vector<std::string>&);

private:
    QGridLayout* layoutView;
    QTextEdit* editor;
    QLabel* nameSetLable;
    QLabel* paramSetLable;
    QLabel* sectionNameSetLabel;

    QLineEdit* testNameEdit;
    QLineEdit* testTageEdit;
    QLineEdit* paramEdit;
    QLineEdit* sectionSetEdit;
    QLineEdit* sectionCheckEdit;

    QPushButton* buttonCreateTestCode;
    QPushButton* buttonCreateMock;

    const MyFunction::SourceCodeFunctionMessageMap& functionMessage;
    std::string funcNameSelected;
};