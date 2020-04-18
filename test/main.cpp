//#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch2/catch.hpp"

#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>

#include "clang-c/Index.h"

char *findingString = "Init";
int numOfArgc = 0;

bool printKindSpelling(CXCursor cursor)
{
    enum CXCursorKind curKind = clang_getCursorKind(cursor);
    const char *curkindSpelling = clang_getCString(
        clang_getCursorKindSpelling(curKind));
    printf("The AST node kind spelling is:%s\n", curkindSpelling);
    return true;
}

bool printSpelling(CXCursor cursor)
{
    const char *astSpelling = clang_getCString(clang_getCursorSpelling(cursor));
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

enum CXChildVisitResult printVisitor(CXCursor cursor, CXCursor parent,
                                     CXClientData client_data)
{
    const char *astSpelling = clang_getCString(clang_getCursorSpelling(cursor));
    if (numOfArgc == 3)
    {
        if (strcmp(astSpelling, findingString) == 0)
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

int main(int argc, char *argv[])
{
    if ((argc > 3) || (argc < 2))
    {
        printf("You input wrong number arguments!\n");
        return -1;
    }

    if ((strcmp(argv[1], "-help")) == 0)
    {
        printf("scread <filename>              The scread will output all the AST nodes' information\n");
        printf("scread <filename> <keyword>    The scread will output the AST nodes' matched the keyword.\n");
        printf("scread -v                      The scread will output the version information.\n");
        printf("scread -help                   The scread will output the help information.\n");
        return 0;
    }
    else
    {
        FILE *result = fopen(argv[1], "r");
        if (result == NULL)
        {
            printf("Can't open the file: %s.\n", argv[1]);
            return -1;
        }
    }

    numOfArgc = argc;
    if (numOfArgc == 3)
    {
        findingString = argv[2];
    }

    CXIndex Index = clang_createIndex(0, 0);
    CXTranslationUnit TU = clang_parseTranslationUnit(Index, 0, argv, argc,
                                                      0, 0,
                                                      CXTranslationUnit_None);
    CXCursor C = clang_getTranslationUnitCursor(TU);

    printSpelling(C);
    printKindSpelling(C);
    printLocation(C);

    clang_visitChildren(C, printVisitor, NULL);

    clang_disposeTranslationUnit(TU);
    clang_disposeIndex(Index);
    return 0;
}