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

    void ToSaver(int argc, char* argv[])
    {
        for (int i = 0; i < argc; i++)
        {
            Saver.emplace_back(argv[i]);
        }
    }

    const std::vector<std::string> getSaver() { return Saver; }

private:
    std::vector<std::string> Saver;
};
#endif