#include <QApplication>
#include <iostream>
#include <string>

#include "python/Python.h"
#include "spdlog/spdlog.h"
#include "window/mainwindow.h"

using namespace std;
using namespace spdlog;

void pythonCallTest()
{
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('../Src/function/UnitTestCodeProduce')");
    PyObject* pModule = PyImport_ImportModule("ProduceCodeFunc");

    if (!pModule)
    {
        cout << "Python get module failed." << endl;
        return;
    }
    cout << "Python get module succeed." << endl;

    PyObject* pFunc = nullptr;
    pFunc = PyObject_GetAttrString(pModule, "MyHelloWorld");
    PyEval_CallObject(pFunc, nullptr);

    PyObject* pv = PyObject_GetAttrString(pModule, "_add");
    if (!pv || !PyCallable_Check(pv))
    {
        cout << "Can't find funftion (_add)" << endl;
        return;
    }
    cout << "Get function (_add) succeed." << endl;

    //初始化要传入的参数，args配置成传入两个参数的模式
    PyObject* args = PyTuple_New(2);
    //将Long型数据转换成Python可接收的类型
    PyObject* arg1 = PyLong_FromLong(4);
    PyObject* arg2 = PyLong_FromLong(3);

    //将arg1配置为arg带入的第一个参数
    PyTuple_SetItem(args, 0, arg1);
    //将arg1配置为arg带入的第二个参数
    PyTuple_SetItem(args, 1, arg2);

    //传入参数调用函数，并获取返回值
    PyObject* pRet = PyObject_CallObject(pv, args);

    if (pRet)
    {
        //将返回值转换成long型
        long result = PyLong_AsLong(pRet);
        cout << "result:" << result << endl;
    }

    Py_Finalize();
}


int main(int argc, char* argv[])
{
    for (auto i = 0; i < argc; i++)
    {
        info("argv[{}] = {}\n", i, argv[i]);
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    pythonCallTest();
    return a.exec();
}
