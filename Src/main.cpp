#include <QApplication>
#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include "window/mainwindow.h"

using namespace std;
using namespace spdlog;


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
