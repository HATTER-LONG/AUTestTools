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
     * @brief Get the Class Product ID object
     *
     * @return std::string
     */
    virtual std::string GetClassProductID() = 0;
};

class DerivedClass : public BaseClass
{
public:
    DerivedClass() = default;
    ~DerivedClass() override = default;

    std::string GetClassProductID() override { return ProductID(); };
    static std::string ProductID() { return "DerivedClassProduct"; }
};

class DerivedAnotherClass : public BaseClass
{
public:
    DerivedAnotherClass() = default;
    ~DerivedAnotherClass() override = default;

    std::string GetClassProductID() override { return ProductID(); };
    static std::string ProductID() { return "DerivedAnotherClassProduct"; }
};

using g_BaseClassFactory = Infra::ProductClassFactory<class BaseClass>;

TEST_CASE("Factory register Product functions", "[factory template test]")
{
    GIVEN("Factory without products")
    {
        WHEN("No product registration, but try to obtain the product")
        {
            auto oneProduct4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProductID());
            THEN("Check the nullptr returned") { REQUIRE(oneProduct4DerivedClass == nullptr); }
        }
        WHEN("regist DerivedClass Product to Factory")
        {
            Infra::ProductClassRegistrar<class BaseClass, class DerivedClass> registProduct(DerivedClass::ProductID());
            THEN("Try get a DerivedClass Product and check return value")
            {
                auto oneProduct4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProductID());
                REQUIRE(oneProduct4DerivedClass != nullptr);
                REQUIRE(oneProduct4DerivedClass->GetClassProductID() == DerivedClass::ProductID());
            }
        }
        WHEN("after local var register DerivedClass to factory try get the Product again")
        {
            auto oneProduct4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProductID());
            THEN("Check the return value") { REQUIRE(oneProduct4DerivedClass == nullptr); }
        }
    }

    GIVEN("Factory with a product")
    {
        Infra::ProductClassRegistrar<class BaseClass, class DerivedClass> registProduct(DerivedClass::ProductID());
        WHEN("Register another product to the factory")
        {
            Infra::ProductClassRegistrar<class BaseClass, class DerivedAnotherClass> registanotherProduct(
                DerivedAnotherClass::ProductID());
            THEN("Gets the product with new Product ID and verifies return values")
            {
                auto oneProduct4AnotherDerivedClass =
                    g_BaseClassFactory::instance().GetProductClass(DerivedAnotherClass::ProductID());
                REQUIRE(oneProduct4AnotherDerivedClass != nullptr);
                REQUIRE(oneProduct4AnotherDerivedClass->GetClassProductID() == DerivedAnotherClass::ProductID());
            }
            THEN("try to Gets old Product")
            {
                auto oneProduct4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProductID());
                REQUIRE(oneProduct4DerivedClass != nullptr);
                REQUIRE(oneProduct4DerivedClass->GetClassProductID() == DerivedClass::ProductID());
            }
        }
        WHEN("Register a product with a duplicate name to the factory")
        {
            Infra::ProductClassRegistrar<class BaseClass, class DerivedAnotherClass> registanotherProduct(
                DerivedClass::ProductID());
            THEN("Gets the product with the duplicate name and verifies that it was not inserted successfully")
            {
                auto oneProduct4DerivedClass = g_BaseClassFactory::instance().GetProductClass(DerivedClass::ProductID());
                REQUIRE(oneProduct4DerivedClass != nullptr);
                REQUIRE(oneProduct4DerivedClass->GetClassProductID() == DerivedClass::ProductID());
            }
        }
    }
}