#include <clang-c/Index.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>


std::string findingString = "Init";
int numOfArgc = 0;

bool printKindSpelling(CXCursor cursor)
{
    enum CXCursorKind curKind = clang_getCursorKind(cursor);
    const char* curkindSpelling = clang_getCString(clang_getCursorKindSpelling(curKind));
    printf("The AST node kind spelling is:%s\n", curkindSpelling);
    return true;
}

bool printSpelling(CXCursor cursor)
{
    const char* astSpelling = clang_getCString(clang_getCursorSpelling(cursor));
    printf("The AST node spelling is:%s\n", astSpelling);
    return true;
}

bool printLocation(CXCursor cursor)
{
    CXSourceRange range = clang_getCursorExtent(cursor);
    CXSourceLocation startLocation = clang_getRangeStart(range);
    CXSourceLocation endLocation = clang_getRangeEnd(range);
    CXFile file;
    unsigned int line, column, offset;
    clang_getInstantiationLocation(startLocation, &file, &line, &column, &offset);
    printf("Start: Line: %u Column: %u Offset: %u\n", line, column, offset);
    clang_getInstantiationLocation(endLocation, &file, &line, &column, &offset);
    printf("End: Line: %u Column: %u Offset: %u\n", line, column, offset);

    return true;
}

enum CXChildVisitResult printVisitor(CXCursor cursor, CXCursor /*parent*/, CXClientData /*client_data*/)
{
    const char* astSpelling = clang_getCString(clang_getCursorSpelling(cursor));
    const char* astKindSpelling = clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(cursor)));
    if (numOfArgc == 3)
    {
        if (astSpelling == findingString || astKindSpelling == findingString)
        {
            printSpelling(cursor);
            printKindSpelling(cursor);
            printLocation(cursor);
        }
    }
    else
    {
        printSpelling(cursor);
        printKindSpelling(cursor);
        printLocation(cursor);
    }
    return CXChildVisit_Recurse;
}

int UseLibclang2VisitorAST(int argc, const char* argv[])
{
    if ((argc > 3) || (argc < 2))
    {
        printf("You input wrong number arguments!\n");
        return -1;
    }
    FILE* result = fopen(argv[1], "r");
    if (result == nullptr)
    {
        printf("Can't open the file: %s.\n", argv[1]);
        return -1;
    }

    numOfArgc = argc;
    if (numOfArgc == 3)
    {
        findingString = std::string(argv[2]);
    }

    CXIndex Index = clang_createIndex(0, 0);
    CXTranslationUnit TU = clang_parseTranslationUnit(Index, nullptr, argv, argc, nullptr, 0, CXTranslationUnit_None);
    CXCursor C = clang_getTranslationUnitCursor(TU);

    printSpelling(C);
    printKindSpelling(C);
    printLocation(C);

    clang_visitChildren(C, printVisitor, nullptr);

    clang_disposeTranslationUnit(TU);
    clang_disposeIndex(Index);
    return 0;
}