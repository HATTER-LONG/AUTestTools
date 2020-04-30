#include <iostream>
#include <string>

#include "spdlog/spdlog.h"
#include "window/mainwindow.h"
#include "clang-c/Index.h"

#include <QApplication>

std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}
void test()
{
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit =
        clang_parseTranslationUnit(index, "./test.cpp", nullptr, 0, nullptr, 0, CXTranslationUnit_None);
    if (unit == nullptr)
    {
        std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        exit(-1);
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor /*parent*/, CXClientData /*client_data*/) {
            std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
                      << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
            return CXChildVisit_Recurse;
        },
        nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // test();
    std::string Info = "HelloWorld!";
    spdlog::info("AUTest Project {}", Info);
    w.show();
    return a.exec();
}
