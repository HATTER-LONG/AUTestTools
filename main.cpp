#include <iostream>
#include <QApplication>

#include "window/mainwindow.h"
#include "spdlog/spdlog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    std::string Info = "HelloWorld!";
    spdlog::info("AUTest Project {}", Info);

    w.show();
    return a.exec();
}
