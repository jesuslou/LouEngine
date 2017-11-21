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

#include <entity/CEntity.h>
#include <entity/CEntityManager.h>
#include <entity/CPrefabManager.h>
#include <component/CComponent.h>
#include <component/CComponentFactoryManager.h>
#include <handle/CHandle.h>
#include <systems/CSystems.h>

#include <gtest/gtest.h>

class CPrefabManagerTest : public ::testing::Test
{
public:
	CPrefabManagerTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
		, m_entityManager(new CEntityManager)
		, m_prefabManager(nullptr)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);
		m_prefabManager = new CPrefabManager();
		m_gameSystems.SetSystem<CPrefabManager>(m_prefabManager);
	}

	~CPrefabManagerTest()
	{
		m_gameSystems.DestroySystem<CPrefabManager>();
		m_gameSystems.DestroySystem<CEntityManager>();
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CComponentFactoryManager *m_componentFactoryManager;
	CEntityManager* m_entityManager;
	CPrefabManager* m_prefabManager;
};

TEST_F(CPrefabManagerTest, register_prefab)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = m_prefabManager->RegisterPrefab("test", entity);
	EXPECT_TRUE(success);
}

TEST_F(CPrefabManagerTest, register_existing_prefab_fails)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = m_prefabManager->RegisterPrefab("test", entity);
	EXPECT_TRUE(success);
	success = m_prefabManager->RegisterPrefab("test", entity);
	EXPECT_FALSE(success);
}

TEST_F(CPrefabManagerTest, clone_prefab)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	entity->SetName("test1");

	bool success = m_prefabManager->RegisterPrefab("test", entity);
	EXPECT_TRUE(success);

	CEntity* cloned = m_prefabManager->ClonePrefab("test");
	EXPECT_NE(nullptr, cloned);
	EXPECT_EQ(cloned->GetName(), entity->GetName());
}

TEST_F(CPrefabManagerTest, clone_invalid_prefab_return_invalid_entity)
{
	CEntity* cloned = m_prefabManager->ClonePrefab("test");
	EXPECT_EQ(nullptr, cloned);
}
