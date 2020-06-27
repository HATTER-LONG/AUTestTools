#pragma once
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class MineEditWindow : public QWidget
{
    Q_OBJECT
public:
    MineEditWindow(QWidget* parent = nullptr);

public slots:
    void createSelectFuncTestCode(QString funcname);

private:
    QTextEdit* editor;
};