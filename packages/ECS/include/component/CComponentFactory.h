#pragma once

#include <component/CComponent.h>
#include <utils/CVersionableFactory.h>

class IComponentFactory 
{
public:
	virtual CComponent* CreateComponent() = 0;
};

template <typename T>
class CComponentFactory : public CVersionable, public IComponentFactory
{
	static_assert(
		std::is_base_of<CComponent, T>::value,
		"T must inherits from CComponent"
		);
public:
	CComponentFactory(int nElements)
		: m_componentPool(nElements)
	{
	}

	CComponent* CreateComponent() override
	{
		return m_componentPool.Get();
	}

	void DestroyComponent(T* component)
	{
		m_componentPool.Destroy(&component);
	}
private:
	static constexpr int MAX_COMPONENTS = 8192;
	CVersionableFactory<T> m_componentPool;
};
