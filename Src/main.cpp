#include <QApplication>
#include <iostream>
#include <string>


#include "Function/AnalyseCodeTree.h"
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
    std::string Info = "HelloWorld!";
    spdlog::info("AUTest Project {}", Info);
    w.show();
    return a.exec();
}
