#include <QApplication>
#include <iostream>
#include <string>


#include "spdlog/spdlog.h"
#include "window/mainwindow.h"

using namespace std;
using namespace spdlog;
int main(int argc, char* argv[])
{
    for (auto i = 0; i < argc; i++)
    {
        info("argv[{}] = {}\n", i, argv[i]);
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
