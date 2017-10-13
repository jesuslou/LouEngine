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
#include <component/CComponent.h>
#include <component/CComponentFactoryManager.h>
#include <handle/CHandle.h>
#include <systems/CSystems.h>

#include <gtest/gtest.h>

class CEntityManagerTest : public ::testing::Test
{
public:
	CEntityManagerTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
		, m_entityManager(new CEntityManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);
	}

	~CEntityManagerTest()
	{
		m_gameSystems.DestroySystem<CEntityManager>();
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CEntityManager *m_entityManager;
	CComponentFactoryManager *m_componentFactoryManager;
};

TEST_F(CEntityManagerTest, destroy_entity_with_entity)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	m_entityManager->DestroyEntity(&entity);
	EXPECT_EQ(nullptr, entity);
}

TEST_F(CEntityManagerTest, destroy_entity_with_handle)
{
	CHandle entity = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(entity));
	m_entityManager->DestroyEntity(entity);
	EXPECT_FALSE(static_cast<bool>(entity));
}
