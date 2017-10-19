////////////////////////////////////////////////////////////
//
// LouEngine - Multiplatform Game Engine Project
// Copyright (C) 2016-2017 Jesus Lou (loucocito@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

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
	CComponent* AddComponent(std::vector<CComponent*>& components)
	{
		int factoryIdx = GetFactoryindex<T>();
		if (factoryIdx >= 0)
		{
			IComponentFactory* factory = GetFactory<T>();
			if (factory)
			{
				components[factoryIdx] = factory->CreateComponent();
				return components[factoryIdx];
			}
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

	template<typename T>
	int GetFactoryindex()
	{
		long long id = CTypeHasher::Hash<T>();
		for (std::size_t i = 0; i < m_factories.size(); ++i)
		{
			if (m_factories[i].m_id == id)
			{
				return i;
			}
		}
		return 1;
	}

	CComponent* CreateComponent(CStrID componentNameId);
	CComponent* CloneComponent(CComponent* component, int componentIdx);
	void CloneComponents(std::vector<CComponent*>& src, std::vector<CComponent*>& dst);
	CComponent* AddComponent(CStrID componentNameId, std::vector<CComponent*>& components);
	CComponent* Get(std::size_t componentTypeIdx, int index, int version);
	CHandle SetHandleInfoFromComponent(CComponent* component);
	int GetPositionForElement(CComponent* component);
	bool DestroyComponent(CComponent** component);

	int GetFactoryIndexByName(CStrID nameId);

	int GetRegisteredComponentsAmount() const { return m_factories.size(); }

	void Update(float dt);

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

	IComponentFactory* GetFactoryByName(CStrID nameId);
	IComponentFactory* GetFactoryByIndex(std::size_t idx);

	std::vector<SFactoryData> m_factories;
};
