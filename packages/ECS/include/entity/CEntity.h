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

#include <handle/CHandle.h>
#include <common/CECSElement.h>
#include <hash/CStrID.h>

#include <vector>

class CComponent;
class CComponentFactoryManager;

class CEntity : public CECSElement
{
	template<class CEntity> friend class CFactory;
public:
	operator CHandle();
	const CEntity* operator=(const CHandle& rhs);

	template<typename T>
	CHandle AddComponent()
	{
		CComponent* component = m_componentFactoryManager.AddComponent<T>(m_components);
		if (component)
		{
			component->SetOwner(this);
		}
		return component;
	}
	template<typename T>
	bool RemoveComponent()
	{
		if (!m_components.empty())
		{
			int componentIdx = m_componentFactoryManager.GetFactoryindex<T>();
			if (m_components[componentIdx])
			{
				return m_componentFactoryManager.DestroyComponent(&m_components[componentIdx]);
			}
		}
		return false;
	}
	template<typename T>
	CHandle GetComponent()
	{
		int componentIdx = m_componentFactoryManager.GetFactoryindex<T>();
		if (componentIdx >= 0)
		{
			return m_components[componentIdx];
		}
		return CHandle();
	}

	CHandle AddComponent(CStrID nameId);
	bool RemoveComponent(CStrID nameId);
	CHandle GetComponent(CStrID nameId);

	void Init() override;
	void Destroy() override;

	void Activate() override;
	void Deactivate() override;

	bool IsInitialized() const { return m_initialized; }
	bool IsActive() const { return m_numDeactivations == 0; }
	bool IsDestroyed() const { return m_destroyed; }

private:
	CEntity();
	~CEntity();

	std::vector<CComponent*> m_components;

	CComponentFactoryManager& m_componentFactoryManager;

	int m_numDeactivations;
	bool m_initialized;
	bool m_destroyed;
};
