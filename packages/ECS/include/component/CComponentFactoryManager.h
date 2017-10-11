#pragma once

#include <component/CComponent.h>
#include <component/CComponentFactory.h>
#include <utils/CTypeHasher.h>
#include <hash/CStrID.h>
#include <systems/CSystems.h>

#include <vector>

#define ADD_COMPONENT_FACTORY(name, T, N) CSystems::GetSystem<CComponentFactoryManager>()->AddFactory(name, new CComponentFactory<T>(N))

class CComponentFactoryManager
{
	struct SFactoryData
	{
		long long m_id;
		CStrID m_nameId;
		IComponentFactory* m_address;
		SFactoryData() : m_id(0), m_address(nullptr) {}
		SFactoryData(long long id, const char* const nameId, IComponentFactory* address) : m_id(id), m_nameId(nameId), m_address(address) {}
	};
public:
	template<typename T>
	typename std::enable_if<std::is_base_of<CComponent, T>::value>::type
	AddFactory(const char* const factoryName, CComponentFactory<T>* factory)
	{
		if (!FactoryExists<T>())
		{
			m_factories.emplace_back(CTypeHasher::Hash<T>(), factoryName, factory);
		}
	}

	template<typename T>
	CComponent* CreateComponent()
	{
		IComponentFactory* factory = GetFactory<T>();
		if (factory)
		{
			return factory->CreateComponent();
		}
		return nullptr;
	}

	template<typename T>
	CComponent* GetByIdxAndVersion(int index, int version)
	{
		IComponentFactory* factory = GetFactory<T>();
		if (factory)
		{
			return factory->GetByIdxAndVersion(index, version);
		}
		return nullptr;
	}

	CComponent* Get(std::size_t componentTypeIdx, int index, int version)
	{
		if (componentTypeIdx < m_factories.size())
		{
			IComponentFactory* factory = m_factories[componentTypeIdx].m_address;
			if (factory)
			{
				return factory->GetByIdxAndVersion(index, version);
			}
		}
		return nullptr;
	}

	CHandle SetHandleInfoFromComponent(CComponent* component)
	{
		CHandle handle;
		for (std::size_t i = 0; i < m_factories.size(); ++i)
		{
			if (m_factories[i].m_address->SetHandleInfoFromComponent(component, handle))
			{
				handle.m_componentIdx = i;
			}
		}
		return handle;
	}

	int GetPositionForElement(CComponent* component)
	{
		for (std::size_t i = 0; i < m_factories.size(); ++i)
		{
			int pos = m_factories[i].m_address->GetComponentPosition(component);
			if (pos >= 0)
			{
				return pos;
			}
		}
		return -1;
	}

	void DestroyComponent(CComponent** component)
	{
		for (std::size_t i = 0; i < m_factories.size(); ++i)
		{
			if (m_factories[i].m_address->DestroyComponent(*component))
			{
				*component = nullptr;
				return;
			}
		}
	}

private:

	template<typename T>
	bool FactoryExists()
	{
		long long id = CTypeHasher::Hash<T>();
		for (SFactoryData& data : m_factories)
		{
			if (data.m_id == id)
			{
				return true;
			}
		}
		return false;
	}

	template<typename T>
	IComponentFactory* GetFactory()
	{
		long long id = CTypeHasher::Hash<T>();
		for (SFactoryData& data : m_factories)
		{
			if (data.m_id == id)
			{
				return data.m_address;
			}
		}
		return nullptr;
	}

	IComponentFactory* GetFactoryByName(CStrID nameId)
	{
		for (SFactoryData& data : m_factories)
		{
			if (data.m_nameId == nameId)
			{
				return data.m_address;
			}
		}
		return nullptr;
	}

	IComponentFactory* GetFactoryByIndex(std::size_t idx)
	{
		if (idx < m_factories.size())
		{
			return m_factories[idx].m_address;
		}
		return nullptr;
	}

	std::vector<SFactoryData> m_factories;
};
