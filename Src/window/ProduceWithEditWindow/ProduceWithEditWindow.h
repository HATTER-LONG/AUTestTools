#pragma once
#include "qstatictext.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QGridLayout;
class QButton;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE


class ProduceWithEditWindow : public QWidget
{
    Q_OBJECT
public:
    ProduceWithEditWindow(QWidget* parent = nullptr);

public slots:
    void createSelectFuncTestCode(QString funcname);

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

    QButton* buttonCreateMock;
};