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
#include <component/CComponentFactoryManager.h>
#include <handle/CHandle.h>
#include <systems/CSystems.h>

#include <gtest/gtest.h>

class CEntityTest : public ::testing::Test
{
public:
	CEntityTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
		, m_entityManager(new CEntityManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);
	}

	~CEntityTest()
	{
		m_gameSystems.DestroySystem<CEntityManager>();
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CEntityManager *m_entityManager;
	CComponentFactoryManager *m_componentFactoryManager;
};

TEST_F(CEntityTest, entity_set_name)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	static const std::string entityName("TestName");
	entity->SetName(entityName.c_str());
	const std::string& name = entity->GetName();

	EXPECT_EQ(name, entityName);
}

TEST_F(CEntityTest, entity_created_uninitalized_and_deactivated)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);

	EXPECT_FALSE(entity->IsInitialized());
	EXPECT_FALSE(entity->IsActive());
}

TEST_F(CEntityTest, entity_not_activated_on_initialization)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);

	EXPECT_FALSE(entity->IsInitialized());
	EXPECT_FALSE(entity->IsActive());
	entity->Init();
	EXPECT_TRUE(entity->IsInitialized());
	EXPECT_FALSE(entity->IsActive());
}

TEST_F(CEntityTest, entity_add_child_as_entity)
{
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
	EXPECT_TRUE(parent->HasChild(child));

	CEntity* childParent = child->GetParent();
	EXPECT_EQ(childParent, parent);
}

TEST_F(CEntityTest, entity_add_child_as_handle)
{
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CHandle child = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(child));

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
	EXPECT_TRUE(parent->HasChild(child));

	CEntity* childEntity = child;
	CEntity* childParent = childEntity->GetParent();
	EXPECT_EQ(childParent, parent);
}

TEST_F(CEntityTest, entity_add_child_twice_fails)
{
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CHandle child = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(child));

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
	EXPECT_TRUE(parent->HasChild(child));
	success = parent->AddChild(child);
	EXPECT_FALSE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
}

TEST_F(CEntityTest, entity_remove_child_as_entity)
{
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	success = parent->RemoveChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(0, parent->GetChildrenCount());
	EXPECT_FALSE(parent->HasChild(child));

	CEntity* childParent = child->GetParent();
	EXPECT_EQ(nullptr, childParent);
}

TEST_F(CEntityTest, entity_remove_child_as_handle)
{
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CHandle child = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(child));

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	success = parent->RemoveChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(0, parent->GetChildrenCount());
	EXPECT_FALSE(parent->HasChild(child));

	CEntity* childEntity = child;
	CHandle childParent = childEntity->GetParent();
	EXPECT_FALSE(static_cast<bool>(childParent));
}

TEST_F(CEntityTest, entity_remove_child_twice_fails)
{
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	success = parent->RemoveChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(0, parent->GetChildrenCount());
	EXPECT_FALSE(parent->HasChild(child));
	success = parent->RemoveChild(child);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTest, get_children_by_name)
{
	static const char* const CHILD_NAME("Test");
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);
	child->SetName(CHILD_NAME);

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
	EXPECT_TRUE(parent->HasChild(child));

	CHandle childHandle = parent->GetChildByName(CHILD_NAME);
	EXPECT_TRUE(static_cast<bool>(childHandle));
	child = childHandle;
	EXPECT_EQ(child->GetName(), std::string(CHILD_NAME));
}

TEST_F(CEntityTest, get_children_by_invalid_name_return_invalid_handle)
{
	static const char* const CHILD_NAME("Test");
	static const char* const WRONG_NAME("Test2");
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);
	child->SetName("Test");

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
	EXPECT_TRUE(parent->HasChild(child));

	CHandle childHandle = parent->GetChildByName(WRONG_NAME);
	EXPECT_FALSE(static_cast<bool>(childHandle));
}

TEST_F(CEntityTest, get_children_by_index)
{
	static const char* const CHILD_NAME("Test");
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);
	child->SetName(CHILD_NAME);

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
	EXPECT_TRUE(parent->HasChild(child));

	CHandle childHandle = parent->GetChildByIndex(0);
	EXPECT_TRUE(static_cast<bool>(childHandle));
	child = childHandle;
	EXPECT_EQ(child->GetName(), std::string(CHILD_NAME));
}

TEST_F(CEntityTest, get_children_by_invalid_index_return_invalid_handle)
{
	static const char* const CHILD_NAME("Test");
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);
	child->SetName(CHILD_NAME);

	bool success = parent->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent->GetChildrenCount());
	EXPECT_TRUE(parent->HasChild(child));

	CHandle childHandle = parent->GetChildByIndex(1);
	EXPECT_FALSE(static_cast<bool>(childHandle));
}

TEST_F(CEntityTest, get_children_by_index_with_no_children)
{
	static const char* const CHILD_NAME("Test");
	CEntity* parent = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent);

	CHandle childHandle = parent->GetChildByIndex(1);
	EXPECT_FALSE(static_cast<bool>(childHandle));
}

TEST_F(CEntityTest, entity_change_entity_parent)
{
	CEntity* parent1 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent1);
	CEntity* parent2 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, parent2);
	CEntity* child = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child);

	bool success = parent1->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent1->GetChildrenCount());
	EXPECT_TRUE(parent1->HasChild(child));
	CEntity* childParent = child->GetParent();
	EXPECT_EQ(parent1, childParent);

	success = parent2->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parent2->GetChildrenCount());
	EXPECT_TRUE(parent2->HasChild(child));
	childParent = child->GetParent();
	EXPECT_EQ(parent2, childParent);

	EXPECT_EQ(0, parent1->GetChildrenCount());
	EXPECT_FALSE(parent1->HasChild(child));
}

TEST_F(CEntityTest, entity_delete_also_deletes_children)
{
	CHandle parent = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(parent));
	CHandle child = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(child));

	CEntity* parentEntity = parent;
	bool success = parentEntity->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parentEntity->GetChildrenCount());
	EXPECT_TRUE(parentEntity->HasChild(child));

	m_entityManager->DestroyEntity(parent);
	EXPECT_FALSE(static_cast<bool>(parent));
	EXPECT_FALSE(static_cast<bool>(child));
}

TEST_F(CEntityTest, entity_delete_removes_itself_from_its_parent)
{
	CHandle parent = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(parent));
	CHandle child = m_entityManager->GetNewElement();
	EXPECT_TRUE(static_cast<bool>(child));

	CEntity* parentEntity = parent;
	bool success = parentEntity->AddChild(child);
	EXPECT_TRUE(success);
	EXPECT_EQ(1, parentEntity->GetChildrenCount());
	EXPECT_TRUE(parentEntity->HasChild(child));

	m_entityManager->DestroyEntity(child);
	EXPECT_FALSE(static_cast<bool>(child));

	EXPECT_EQ(0, parentEntity->GetChildrenCount());
	EXPECT_FALSE(parentEntity->HasChild(child));
}
