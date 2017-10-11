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

#include <entity/CEntityManager.h>
#include <entity/CEntity.h>
#include <handle/CHandle.h>
#include <component/CComponentFactoryManager.h>
#include <systems/CSystems.h>

#include <gtest/gtest.h>

namespace VersionableFactoryTestInternal
{
	class Foo : public CVersionable
	{
	};
}

class CEntityHandleTest : public ::testing::Test
{
public:
	CEntityHandleTest()
		: m_entityManager(new CEntityManager)
		, m_componentFactoryManager(new CComponentFactoryManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
	}

	~CEntityHandleTest()
	{
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
		m_gameSystems.DestroySystem<CEntityManager>();
	}

	CGameSystems m_gameSystems;
	CEntityManager *m_entityManager;
	CComponentFactoryManager *m_componentFactoryManager;
};

TEST_F(CEntityHandleTest, get_new_entity_pointer)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
}

TEST_F(CEntityHandleTest, get_new_entity_as_handle)
{
	CHandle entityHandle = m_entityManager->GetNewElement();
	EXPECT_EQ(CHandle::EElementType::Entity, entityHandle.m_elementType);
	EXPECT_EQ(0, entityHandle.m_componentIdx);
	EXPECT_EQ(0, entityHandle.m_elementPosition);
	EXPECT_EQ(0, entityHandle.m_version);
	EXPECT_TRUE(static_cast<bool>(entityHandle));

	CHandle entityHandle2 = m_entityManager->GetNewElement();
	EXPECT_EQ(CHandle::EElementType::Entity, entityHandle2.m_elementType);
	EXPECT_EQ(0, entityHandle2.m_componentIdx);
	EXPECT_EQ(1, entityHandle2.m_elementPosition);
	EXPECT_EQ(0, entityHandle2.m_version);
	EXPECT_TRUE(static_cast<bool>(entityHandle));
}

TEST_F(CEntityHandleTest, cast_handle_to_entity)
{
	CHandle entityHandle = m_entityManager->GetNewElement();

	CEntity* entity = entityHandle;
	EXPECT_NE(nullptr, entity);

	int pos = m_entityManager->GetPositionForElement(entity);
	EXPECT_EQ(pos, entityHandle.m_elementPosition);
	EXPECT_EQ(entity->GetVersion(), entityHandle.m_version);
	CEntity* entity2 = m_entityManager->GetElementByIdxAndVersion(pos, entity->GetVersion());
	EXPECT_EQ(entity2, entity);
}

TEST_F(CEntityHandleTest, cast_entity_to_handle)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);

	CHandle entityHandle = entity;
	EXPECT_EQ(CHandle::EElementType::Entity, entityHandle.m_elementType);
	EXPECT_EQ(0, entityHandle.m_componentIdx);
	EXPECT_EQ(0, entityHandle.m_elementPosition);
	EXPECT_EQ(0, entityHandle.m_version);
	EXPECT_TRUE(static_cast<bool>(entityHandle));
}

TEST_F(CEntityHandleTest, invalidate_handle)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);

	CHandle entityHandle = entity;
	EXPECT_TRUE(static_cast<bool>(entityHandle));
	m_entityManager->DestroyElement(&entity);
	EXPECT_FALSE(static_cast<bool>(entityHandle));
}

TEST_F(CEntityHandleTest, null_entity_from_invalid_handle)
{
	CHandle handle;
	EXPECT_FALSE(static_cast<bool>(handle));
	CEntity* entity = handle;
	EXPECT_EQ(nullptr, entity);
}

TEST_F(CEntityHandleTest, invalid_handle_from_null_entity)
{
	CEntity* entity = nullptr;
	CHandle handle = entity;
	EXPECT_FALSE(static_cast<bool>(handle));
}

TEST_F(CEntityHandleTest, invalid_cast_to_handle_after_destroy)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CHandle handle = entity;
	EXPECT_TRUE(static_cast<bool>(handle));

	m_entityManager->DestroyElement(&entity);

	EXPECT_FALSE(static_cast<bool>(handle));
	entity = handle;
	EXPECT_EQ(nullptr, entity);
}
