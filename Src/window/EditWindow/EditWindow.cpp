#include "EditWindow.h"

MineEditWindow::MineEditWindow(QWidget* parent) : QWidget(parent)
{
    editor = new QTextEdit(this);
    auto* layout = new QVBoxLayout();
    layout->addWidget(editor);
    this->setLayout(layout);
}

void MineEditWindow::createSelectFuncTestCode(QString funcname) { editor->append(funcname); }