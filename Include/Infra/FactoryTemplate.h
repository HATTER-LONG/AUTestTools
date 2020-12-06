#pragma once

#include "spdlog/spdlog.h"
#include <map>
#include <string>
namespace Infra
{
/**
 * @brief 工厂产品注册基类
 *
 * @tparam CustomProductType_t 产品基类类型
 */
template <class CustomProductType_t>
class IProductClassRegistrar
{
public:
    /**
     * @brief Create a Product object
     *
     * @return std::unique_ptr<CustomProductType_t>
     */
    virtual std::unique_ptr<CustomProductType_t> CreateProduct() = 0;

    IProductClassRegistrar(const IProductClassRegistrar&) = delete;
    const IProductClassRegistrar& operator=(const IProductClassRegistrar&) = delete;

protected:
    IProductClassRegistrar() = default;
    virtual ~IProductClassRegistrar() = default;
};

/**
 * @brief 模板工厂类
 *
 * @tparam CustomProductType_t
 */
template <class CustomProductType_t>
class ProductClassFactory
{
public:
    /**
     * @brief 对应具体实例模板工厂单例获取
     *
     * @return ProductClassFactory<CustomProductType_t>&
     */
    static ProductClassFactory<CustomProductType_t>& instance()
    {
        static ProductClassFactory<CustomProductType_t> instance;
        return instance;
    }

    /**
     * @brief 通过产品 ID 注册到对应具体实例的模板工厂中
     *
     * @param registry
     * @param ID
     */
    void RegisterClassWithID(IProductClassRegistrar<CustomProductType_t>* registry, std::string ID)
    {
        auto iter = m_ProductClassRegistry.find(ID);
        if (iter != m_ProductClassRegistry.end())
        {
            spdlog::warn("[{}] Error with Repeatedly insert the class with ID[{}] into the factory, pls check it", __FUNCTION__,
                         ID.c_str());

            return;
        }
        m_ProductClassRegistry[ID] = registry;
    }

    /**
     * @brief Get the Product Class object
     *
     * @param ID
     * @return std::unique_ptr<CustomProductType_t>
     */
    std::unique_ptr<CustomProductType_t> GetProductClass(std::string ID)
    {
        if (m_ProductClassRegistry.find(ID) != m_ProductClassRegistry.end())
        {
            return m_ProductClassRegistry[ID]->CreateProduct();
        }
        spdlog::warn("{} No product class found for ID[{}]", __FUNCTION__, ID.c_str());
        return std::unique_ptr<CustomProductType_t>(nullptr);
    }

    ProductClassFactory(const ProductClassFactory&) = delete;
    const ProductClassFactory& operator=(const ProductClassFactory&) = delete;

private:
    ProductClassFactory() = default;
    ~ProductClassFactory() = default;

    /**
     * @brief 保存注册过的产品，key:产品名字 , value:产品类型存
     *
     */
    std::map<std::string, IProductClassRegistrar<CustomProductType_t>*> m_ProductClassRegistry;
};

/**
 * @brief 产品注册模板类，用于创建具体产品和从工厂里注册产品
 *        模板参数 CustomProductType_t 表示的类是产品抽象类（基类），CustomProductImpl_t 表示的类是具体产品（产品种类的子类）
 *
 * @tparam CustomProductType_t
 * @tparam CustomProductImpl_t
 */
template <class CustomProductType_t, class CustomProductImpl_t>
class ProductClassRegistrar : IProductClassRegistrar<CustomProductType_t>
{
public:
    /**
     * @brief Construct a new Product Class Registrar object
     *
     * @param ID
     */
    explicit ProductClassRegistrar(std::string ID)
    {
        ProductClassFactory<CustomProductType_t>::instance().RegisterClassWithID(this, ID);
    }

    /**
     * @brief Create a Product object
     *
     * @return std::unique_ptr<CustomProductType_t>
     */
    std::unique_ptr<CustomProductType_t> CreateProduct()
    {
        return std::unique_ptr<CustomProductType_t>(std::make_unique<CustomProductImpl_t>());
    }
};
} // namespace Infra