#pragma once

#include <component/CComponent.h>
#include <common/CFactory.h>

class IComponentFactory 
{
public:
	virtual CComponent* CreateComponent() = 0;
	virtual CComponent* GetByIdxAndVersion(int index, int version) = 0;
	virtual bool SetHandleInfoFromComponent(CComponent* component, CHandle& handle) = 0;
	virtual int GetComponentPosition(CComponent* component) = 0;
	virtual bool DestroyComponent(CComponent* component) = 0;
};

template <typename T>
class CComponentFactory : public IComponentFactory, public CFactory<T>
{
	static_assert(
		std::is_base_of<CComponent, T>::value,
		"T must inherits from CComponent"
		);
public:
	CComponentFactory(int nElements)
		: CFactory(nElements)
	{
	}

	CComponent* CreateComponent() override
	{
		return GetNewElement();
	}

	CComponent* GetByIdxAndVersion(int index, int version) override
	{
		return GetElementByIdxAndVersion(index, version);
	}

	bool SetHandleInfoFromComponent(CComponent* component, CHandle& handle) override
	{
		int pos = GetPositionForElement(static_cast<T*>(component));
		if (pos != -1)
		{
			handle.m_elementType = CHandle::EElementType::Component;
			handle.m_elementPosition = pos;
			handle.m_version = component->GetVersion();
			return true;
		}
		return false;
	}

	int GetComponentPosition(CComponent* component) override
	{
		return GetPositionForElement(static_cast<T*>(component));
	}

	bool DestroyComponent(CComponent* component) override
	{
		T* tmp = static_cast<T*>(component);
		return DestroyElement(&tmp);
	}
};
