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

#include <component/CComponentFactoryManager.h>

CComponent* CComponentFactoryManager::CreateComponent(CStrID componentNameId)
{
	IComponentFactory* factory = GetFactoryByName(componentNameId);
	if (factory)
	{
		return factory->CreateComponent();
	}
	return nullptr;
}

CComponent* CComponentFactoryManager::AddComponent(CStrID componentNameId, std::vector<CComponent*>& components)
{
	int factoryIdx = GetFactoryIndexByName(componentNameId);
	if (factoryIdx >= 0)
	{
		IComponentFactory* factory = GetFactoryByIndex(factoryIdx);
		if (factory && components[factoryIdx] == nullptr)
		{
			components[factoryIdx] = factory->CreateComponent();
			return components[factoryIdx];
		}
	}
	return nullptr;
}

CComponent* CComponentFactoryManager::Get(std::size_t componentTypeIdx, int index, int version)
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

CHandle CComponentFactoryManager::SetHandleInfoFromComponent(CComponent* component)
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

int CComponentFactoryManager::GetPositionForElement(CComponent* component)
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

bool CComponentFactoryManager::DestroyComponent(CComponent** component)
{
	for (std::size_t i = 0; i < m_factories.size(); ++i)
	{
		(*component)->Destroy();
		if (m_factories[i].m_address->DestroyComponent(*component))
		{
			*component = nullptr;
			return true;
		}
	}
	return false;
}

IComponentFactory* CComponentFactoryManager::GetFactoryByName(CStrID nameId)
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

IComponentFactory* CComponentFactoryManager::GetFactoryByIndex(std::size_t idx)
{
	if (idx < m_factories.size())
	{
		return m_factories[idx].m_address;
	}
	return nullptr;
}

int CComponentFactoryManager::GetFactoryIndexByName(CStrID nameId)
{
	for (std::size_t i = 0; i < m_factories.size(); ++i)
	{
		if (m_factories[i].m_nameId == nameId)
		{
			return i;
		}
	}
	return -1;
}

void CComponentFactoryManager::Update(float dt)
{
	for (SFactoryData& data : m_factories)
	{
		data.m_address->Update(dt);
	}
}
