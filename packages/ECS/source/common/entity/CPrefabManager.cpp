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

#include <entity/CPrefabManager.h>
#include <entity/CEntityManager.h>
#include <handle/CHandle.h>
#include <systems/CSystems.h>

CPrefabManager::CPrefabManager()
	: m_entityManager(*CSystems::GetSystem<CEntityManager>())
{
}

CPrefabManager::~CPrefabManager()
{
	for (auto& pair : m_prefabs)
	{
		m_entityManager.DestroyEntity(&pair.second);
	}
	m_prefabs.clear();
}

bool CPrefabManager::RegisterPrefab(CStrID nameId, CEntity* prefab)
{
	if (m_prefabs.find(nameId) == m_prefabs.end())
	{
		m_prefabs[nameId] = prefab;
		return true;
	}
	return false;
}

CHandle CPrefabManager::ClonePrefab(CStrID nameId)
{
	auto prefabIt = m_prefabs.find(nameId);
	if (prefabIt != m_prefabs.end())
	{
		CEntity* prefab = (*prefabIt).second;
		CEntity* entity = m_entityManager.GetNewElement();
		entity->CloneFrom(prefab);
		return entity;
	}
	return CHandle();
}
