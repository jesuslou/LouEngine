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
#include <tags/CTagsManager.h>
#include <handle/CHandle.h>
#include <systems/CSystems.h>

#include <gtest/gtest.h>

namespace EntityTagsTestInternal
{
	const CStrID TAG1("test");
	const CStrID TAG2("test2");
	const CStrID TAG3("test3");
	const CStrID INVALID_TAG("invalid");
}

class CEntityTagsTest : public ::testing::Test
{
public:
	CEntityTagsTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
		, m_entityManager(new CEntityManager)
		, m_TagsManager(new CTagsManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
		m_gameSystems.SetSystem<CTagsManager>(m_TagsManager);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);

		m_TagsManager->RegisterTag(EntityTagsTestInternal::TAG1);
		m_TagsManager->RegisterTag(EntityTagsTestInternal::TAG2);
		m_TagsManager->RegisterTag(EntityTagsTestInternal::TAG3);
	}

	~CEntityTagsTest()
	{
		m_gameSystems.DestroySystem<CEntityManager>();
		m_gameSystems.DestroySystem<CTagsManager>();
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CEntityManager *m_entityManager;
	CComponentFactoryManager *m_componentFactoryManager;
	CTagsManager *m_TagsManager;
};

TEST_F(CEntityTagsTest, add_single_tag)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);
}

TEST_F(CEntityTagsTest, add_multiple_tags)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);
}

TEST_F(CEntityTagsTest, remove_single_tag)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);

	success = entity->RemoveTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTagsTest, remove_multiple_tags)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->RemoveTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_FALSE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG2);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTagsTest, remove_single_tag_dont_remove_other_tags)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->RemoveTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_FALSE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);
}

TEST_F(CEntityTagsTest, add_nonexistent_tag_fails)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::INVALID_TAG);
	EXPECT_FALSE(success);

	success = entity->HasTags(EntityTagsTestInternal::INVALID_TAG);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTagsTest, add_nonexistent_tag_dont_prevent_to_add_valid_tags)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::INVALID_TAG, EntityTagsTestInternal::TAG1);
	EXPECT_FALSE(success);

	success = entity->HasTags(EntityTagsTestInternal::INVALID_TAG);
	EXPECT_FALSE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);
}

TEST_F(CEntityTagsTest, remove_nonexistent_tag_fails)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->RemoveTags(EntityTagsTestInternal::INVALID_TAG);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTagsTest, remove_nonexistent_tag_dont_prevent_to_remove_valid_tags)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_TRUE(success);

	success = entity->RemoveTags(EntityTagsTestInternal::INVALID_TAG, EntityTagsTestInternal::TAG1);
	EXPECT_FALSE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTagsTest, entity_has_all_tags)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->HasTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2, EntityTagsTestInternal::TAG3);
	EXPECT_FALSE(success);
	success = entity->HasTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2, EntityTagsTestInternal::INVALID_TAG);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTagsTest, entity_has_any_tag)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	bool success = entity->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);

	success = entity->HasAnyTag(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_TRUE(success);
	success = entity->HasAnyTag(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG3);
	EXPECT_TRUE(success);
	success = entity->HasAnyTag(EntityTagsTestInternal::TAG2, EntityTagsTestInternal::INVALID_TAG);
	EXPECT_TRUE(success);
	success = entity->HasAnyTag(EntityTagsTestInternal::TAG3);
	EXPECT_FALSE(success);
}

TEST_F(CEntityTagsTest, get_entity_children_with_all_tags)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CEntity* child1 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child1);
	CEntity* child2 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child2);
	CEntity* child11 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child11);

	child1->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	child2->AddTags(EntityTagsTestInternal::TAG1);
	child11->AddTags(EntityTagsTestInternal::TAG2, EntityTagsTestInternal::TAG3);

	entity->AddChild(child1);
	entity->AddChild(child2);
	EXPECT_EQ(2, entity->GetChildrenCount());
	child1->AddChild(child11);
	EXPECT_EQ(1, child1->GetChildrenCount());

	std::vector<CHandle> childrenWithTags;

	childrenWithTags = entity->GetChildrenWithTags(EntityTagsTestInternal::TAG1);
	EXPECT_EQ(2, childrenWithTags.size());
	bool child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	bool child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_TRUE(child2Found);

	childrenWithTags.clear();
	childrenWithTags = entity->GetChildrenWithTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_EQ(1, childrenWithTags.size());
	child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_FALSE(child2Found);

	childrenWithTags.clear();
	childrenWithTags = entity->GetChildrenWithTags(EntityTagsTestInternal::TAG3);
	EXPECT_EQ(0, childrenWithTags.size());
}

TEST_F(CEntityTagsTest, get_entity_children_with_any_tag)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CEntity* child1 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child1);
	CEntity* child2 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child2);
	CEntity* child11 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child11);

	child1->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	child2->AddTags(EntityTagsTestInternal::TAG1);
	child11->AddTags(EntityTagsTestInternal::TAG2, EntityTagsTestInternal::TAG3);

	entity->AddChild(child1);
	entity->AddChild(child2);
	EXPECT_EQ(2, entity->GetChildrenCount());
	child1->AddChild(child11);
	EXPECT_EQ(1, child1->GetChildrenCount());

	std::vector<CHandle> childrenWithTags;

	childrenWithTags = entity->GetChildrenWithAnyTag(EntityTagsTestInternal::TAG1);
	EXPECT_EQ(2, childrenWithTags.size());
	bool child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	bool child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_TRUE(child2Found);

	childrenWithTags.clear();
	childrenWithTags = entity->GetChildrenWithAnyTag(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	EXPECT_EQ(2, childrenWithTags.size());
	child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_TRUE(child2Found);

	childrenWithTags.clear();
	childrenWithTags = entity->GetChildrenWithAnyTag(EntityTagsTestInternal::TAG2);
	EXPECT_EQ(1, childrenWithTags.size());
	child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_FALSE(child2Found);

	childrenWithTags.clear();
	childrenWithTags = entity->GetChildrenWithAnyTag(EntityTagsTestInternal::TAG3);
	EXPECT_EQ(0, childrenWithTags.size());
}

TEST_F(CEntityTagsTest, get_entity_children_with_all_tags_recursive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CEntity* child1 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child1);
	CEntity* child2 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child2);
	CEntity* child11 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child11);

	child1->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	child2->AddTags(EntityTagsTestInternal::TAG1);
	child11->AddTags(EntityTagsTestInternal::TAG2, EntityTagsTestInternal::TAG3);

	entity->AddChild(child1);
	entity->AddChild(child2);
	EXPECT_EQ(2, entity->GetChildrenCount());
	child1->AddChild(child11);
	EXPECT_EQ(1, child1->GetChildrenCount());

	std::vector<CHandle> childrenWithTags;

	childrenWithTags = entity->GetChildrenWithTagsRecursive(EntityTagsTestInternal::TAG2);
	EXPECT_EQ(2, childrenWithTags.size());
	bool child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	bool child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_FALSE(child2Found);
	bool child11Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child11)) != childrenWithTags.end();
	EXPECT_TRUE(child11Found);

	childrenWithTags.clear();
	childrenWithTags = entity->GetChildrenWithTagsRecursive(EntityTagsTestInternal::TAG2, EntityTagsTestInternal::TAG3);
	EXPECT_EQ(1, childrenWithTags.size());
	child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_FALSE(child1Found);
	child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_FALSE(child2Found);
	child11Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child11)) != childrenWithTags.end();
	EXPECT_TRUE(child11Found);
}

TEST_F(CEntityTagsTest, get_entity_children_with_any_tag_recursive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CEntity* child1 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child1);
	CEntity* child2 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child2);
	CEntity* child11 = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, child11);

	child1->AddTags(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG2);
	child2->AddTags(EntityTagsTestInternal::TAG1);
	child11->AddTags(EntityTagsTestInternal::TAG2, EntityTagsTestInternal::TAG3);

	entity->AddChild(child1);
	entity->AddChild(child2);
	EXPECT_EQ(2, entity->GetChildrenCount());
	child1->AddChild(child11);
	EXPECT_EQ(1, child1->GetChildrenCount());

	std::vector<CHandle> childrenWithTags;

	childrenWithTags = entity->GetChildrenWithAnyTagRecursive(EntityTagsTestInternal::TAG2);
	EXPECT_EQ(2, childrenWithTags.size());
	bool child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	bool child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_FALSE(child2Found);
	bool child11Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child11)) != childrenWithTags.end();
	EXPECT_TRUE(child11Found);

	childrenWithTags.clear();
	childrenWithTags = entity->GetChildrenWithAnyTagRecursive(EntityTagsTestInternal::TAG1, EntityTagsTestInternal::TAG3);
	EXPECT_EQ(3, childrenWithTags.size());
	child1Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child1)) != childrenWithTags.end();
	EXPECT_TRUE(child1Found);
	child2Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child2)) != childrenWithTags.end();
	EXPECT_TRUE(child2Found);
	child11Found = std::find(childrenWithTags.begin(), childrenWithTags.end(), CHandle(child11)) != childrenWithTags.end();
	EXPECT_TRUE(child11Found);
}
