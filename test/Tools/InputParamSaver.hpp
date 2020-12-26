#ifndef __INPUTPARAMSAVER_HPP__
#define __INPUTPARAMSAVER_HPP__
#include <string>
#include <vector>

class InputParamSaver
{
public:
    static InputParamSaver* instance()
    {
        static InputParamSaver o;
        return &o;
    }

    void toSaver(int Argc, char* Argv[])
    {
        for (int i = 0; i < Argc; i++)
        {
            Saver.emplace_back(Argv[i]);
        }
    }

    const std::vector<std::string> getSaver()
    {
        return Saver;
    }

private:
    std::vector<std::string> Saver;
};
#endif