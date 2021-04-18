MyTestNameSpace2::TestB::TestB() { }
int MyTestNameSpace2::TestB::functionPrivate()
{
    return int {};
}
int MyTestNameSpace2::TestB::functionPublic(int arg1)
{
    return int {};
}
MyTestNameSpace2::TestB::~TestB() { }

int cTypeFunctionByExtern(std::string arg1, int arg2)
{
    return int {};
}