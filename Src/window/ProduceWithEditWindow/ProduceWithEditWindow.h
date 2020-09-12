#pragma once
#include "function/FunctionDeclAnalysis.h"
#include <QWidget>

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
    ProduceWithEditWindow(const MFunction::SourceCodeFunctionMessageMap& functionInfo, QWidget* parent = nullptr);

public slots:
    void createSelectFuncTestCode(std::string funcname);
    void createCodeButtonClicked();

private:
    void createEditWindowItem();

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

    const MFunction::SourceCodeFunctionMessageMap& functionMessage;
};