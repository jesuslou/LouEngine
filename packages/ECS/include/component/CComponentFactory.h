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
#include <common/CFactory.h>

class IComponentFactory 
{
public:
	virtual CComponent* CreateComponent() = 0;
	virtual CComponent* CloneComponent(CComponent* component) = 0;
	virtual CComponent* GetByIdxAndVersion(int index, int version) = 0;
	virtual bool SetHandleInfoFromComponent(CComponent* component, CHandle& handle) = 0;
	virtual int GetComponentPosition(CComponent* component) = 0;
	virtual bool DestroyComponent(CComponent* component) = 0;
	virtual void Update(float dt) = 0;
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

	CComponent* CloneComponent(CComponent* component) override
	{
		T* newComponent = static_cast<T*>(CreateComponent());
		if (newComponent)
		{
			*newComponent = *(static_cast<T*>(component));
			return newComponent;
		}
		return nullptr;
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

	void Update(float dt) override
	{
		for (SEntry& entry : m_entries)
		{
			if (entry.m_used)
			{
				entry.m_data->Update(dt);
			}
		}
	}
};
