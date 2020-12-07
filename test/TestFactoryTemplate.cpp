#include "Infra/FactoryTemplate.h"
#include "catch2/catch.hpp"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
class BaseClass
{
public:
    BaseClass() = default;
    virtual ~BaseClass() = default;

    /**
     * @brief Get the Class Produce ID object
     *
     * @return std::string
     */
    virtual std::string GetClassProduceID() = 0;
};

class DerivedClass : public BaseClass
{
public:
    DerivedClass() = default;
    ~DerivedClass() override = default;

    std::string GetClassProduceID() override { return ProduceID(); };
    static std::string ProduceID() { return "DerivedClassProduce"; }
};

class DerivedAnotherClass : public BaseClass
{
public:
    DerivedAnotherClass() = default;
    ~DerivedAnotherClass() override = default;

    std::string GetClassProduceID() override { return ProduceID(); };
    static std::string ProduceID() { return "DerivedAnotherClassProduce"; }
};

TEST_CASE("Factory register produce", "[factory by template test]")
{
    GIVEN("A Blank Factory")
    {
        using g_BaseClassFactory = Infra::ProductClassFactory<class BaseClass>;
        WHEN("No produce template regist but try get a produce")
        {
            auto oneproduce4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProduceID());
            THEN("Check the nullptr returned") { REQUIRE(oneproduce4DerivedClass == nullptr); }
        }
        WHEN("regist DerivedClass produce to Factory")
        {
            Infra::ProductClassRegistrar<class BaseClass, class DerivedClass> registproduce(DerivedClass::ProduceID());
            THEN("Try get a DerivedClass produce and check return value")
            {
                auto oneproduce4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProduceID());
                REQUIRE(oneproduce4DerivedClass != nullptr);
                REQUIRE(oneproduce4DerivedClass->GetClassProduceID() == DerivedClass::ProduceID());
            }
        }
        WHEN("after local var register DerivedClass to factory try get the produce again")
        {
            auto oneproduce4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProduceID());
            THEN("Check the return value") { REQUIRE(oneproduce4DerivedClass == nullptr); }
        }
    }
    /**
     * TODO:[不同 ID 存取、析构进行测试。]
     *
     */
}