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
#include <entity/CEntityParser.h>
#include <component/CComponent.h>
#include <component/CComponentFactoryManager.h>
#include <tags/CTagsManager.h>
#include <handle/CHandle.h>
#include <systems/CSystems.h>

#include <json/reader.h>
#include <gtest/gtest.h>

namespace EntityParserTestInternal
{
	class CCompFoo : public CComponent
	{
	public:
		CCompFoo() : m_a(-1), m_b(-1) {}
		void ParseAtts(const Json::Value& atts) override
		{
			m_a = !atts["a"].isNull() ? atts["a"].asInt() : -1;
			m_b = !atts["b"].isNull() ? atts["b"].asInt() : -1;
		}
		int m_a;
		int m_b;
	};

	const char* const ONE_LEVEL_ENTITY = "{ \"root\": {"
		"\"type\": \"entity\", \"name\": \"test1\", \"tags\": [\"tag1\", \"tag3\"],"
		"\"components\": [{ \"type\": \"foo\", \"a\": 1, \"b\": -3 }]}}";

	const char* const ONE_LEVEL_ENTITY_INITIALLY_DISABLED = "{ \"root\": {"
		"\"type\": \"entity\", \"name\": \"test1\", \"tags\": [\"tag1\", \"tag3\"], \"initiallyActive\":false,"
		"\"components\": [{ \"type\": \"foo\", \"a\": 1, \"b\": -3 }]}}";

	const char* const ONE_LEVEL_ENTITY_COMPONENT_INITIALLY_DISABLED = "{ \"root\": {"
		"\"type\": \"entity\", \"name\": \"test1\", \"tags\": [\"tag1\", \"tag3\"],"
		"\"components\": [{ \"type\": \"foo\", \"initiallyActive\":false, \"a\": 1, \"b\": -3 }]}}";

	const char* const ONE_LEVEL_ENTITY_INVALID_COMPONENT = "{ \"root\": {"
		"\"type\": \"entity\", \"name\": \"test1\", \"tags\": [\"tag1\", \"tag3\"],"
		"\"components\": [{ \"type\": \"bar\", \"a\": 1, \"b\": -3 }]}}";

	const char* const ONE_LEVEL_ENTITY_INVALID_TAG = "{ \"root\": {"
		"\"type\": \"entity\", \"name\": \"test1\", \"tags\": [\"tag1\", \"tag3\", \"tag4\"],"
		"\"components\": [{ \"type\": \"foo\", \"a\": 1, \"b\": -3 }]}}";

	const char* const TWO_LEVEL_ENTITY = "{ \"root\": {"
		"\"type\": \"entity\", \"name\": \"test1\", \"tags\": [\"tag1\", \"tag3\"],"
		"\"components\": [{ \"type\": \"foo\", \"a\": 1, \"b\": -3 }],"
		"\"children\": [{ \"type\": \"entity\", \"name\": \"test11\", \"tags\": [\"tag1\", \"tag3\"], \"components\": [{ \"type\": \"foo\", \"a\": 1, \"b\": -3 }]}]}}";

	const char* const TWO_LEVEL_ENTITY_INITIALLY_DISABLED = "{ \"root\": {"
		"\"type\": \"entity\", \"name\": \"test1\", \"tags\": [\"tag1\", \"tag3\"],"
		"\"components\": [{ \"type\": \"foo\", \"a\": 1, \"b\": -3 }],"
		"\"children\": [{ \"type\": \"entity\", \"name\": \"test11\", \"initiallyActive\":false, \"tags\": [\"tag1\", \"tag3\"], \"components\": [{ \"type\": \"foo\", \"a\": 1, \"b\": -3 }]}]}}";
}

class CEntityParserTest : public ::testing::Test
{
public:
	CEntityParserTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
		, m_entityManager(new CEntityManager)
		, m_tagManager(new CTagsManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
		m_gameSystems.SetSystem<CTagsManager>(m_tagManager);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);

		ADD_COMPONENT_FACTORY("foo", EntityParserTestInternal::CCompFoo, 10);

		m_tagManager->RegisterTag("tag1");
		m_tagManager->RegisterTag("tag2");
		m_tagManager->RegisterTag("tag3");
	}

	~CEntityParserTest()
	{
		m_gameSystems.DestroySystem<CEntityManager>();
		m_gameSystems.DestroySystem<CTagsManager>();
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CEntityManager *m_entityManager;
	CComponentFactoryManager *m_componentFactoryManager;
	CTagsManager *m_tagManager;
};

TEST_F(CEntityParserTest, parse_entity)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::ONE_LEVEL_ENTITY);
	EXPECT_NE(nullptr, entity);
	EXPECT_EQ(std::string("test1"), entity->GetName());
	EXPECT_TRUE(entity->GetIsInitiallyActive());
	EXPECT_TRUE(entity->IsInitialized());
	EXPECT_TRUE(entity->IsActive());
	EXPECT_FALSE(entity->IsDestroyed());
}

TEST_F(CEntityParserTest, parse_entity_initiallyActive_false)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::ONE_LEVEL_ENTITY_INITIALLY_DISABLED);
	EXPECT_NE(nullptr, entity);
	EXPECT_EQ(std::string("test1"), entity->GetName());
	EXPECT_FALSE(entity->GetIsInitiallyActive());
	EXPECT_TRUE(entity->IsInitialized());
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(entity->IsDestroyed());
}

TEST_F(CEntityParserTest, parse_entity_with_component)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::ONE_LEVEL_ENTITY);
	EXPECT_NE(nullptr, entity);
	EntityParserTestInternal::CCompFoo* component = entity->GetComponent<EntityParserTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);
	EXPECT_TRUE(component->GetIsInitiallyActive());
	EXPECT_TRUE(component->IsInitialized());
	EXPECT_TRUE(component->IsActive());
	EXPECT_FALSE(component->IsDestroyed());
	EXPECT_EQ(1, component->m_a);
	EXPECT_EQ(-3, component->m_b);
}

TEST_F(CEntityParserTest, parse_entity_with_component_initiallyActive_false)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::ONE_LEVEL_ENTITY_COMPONENT_INITIALLY_DISABLED);
	EXPECT_NE(nullptr, entity);
	EntityParserTestInternal::CCompFoo* component = entity->GetComponent<EntityParserTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);
	EXPECT_FALSE(component->GetIsInitiallyActive());
	EXPECT_TRUE(component->IsInitialized());
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component->IsDestroyed());
	EXPECT_EQ(1, component->m_a);
	EXPECT_EQ(-3, component->m_b);
}

TEST_F(CEntityParserTest, parse_entity_with_invalid_component)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::ONE_LEVEL_ENTITY_INVALID_COMPONENT);
	EXPECT_NE(nullptr, entity);
	EntityParserTestInternal::CCompFoo* component = entity->GetComponent<EntityParserTestInternal::CCompFoo>();
	EXPECT_EQ(nullptr, component);
}

TEST_F(CEntityParserTest, parse_entity_with_tags)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::ONE_LEVEL_ENTITY);
	EXPECT_NE(nullptr, entity);
	EXPECT_TRUE(entity->HasTags("tag1", "tag3"));
	EXPECT_FALSE(entity->HasTags("tag1", "tag2"));
}

TEST_F(CEntityParserTest, parse_entity_with_invalid_tag)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::ONE_LEVEL_ENTITY_INVALID_TAG);
	EXPECT_NE(nullptr, entity);
	EXPECT_TRUE(entity->HasTags("tag1", "tag3"));
	EXPECT_FALSE(entity->HasTags("tag4"));
}

TEST_F(CEntityParserTest, parse_entity_with_child)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::TWO_LEVEL_ENTITY);
	EXPECT_NE(nullptr, entity);
	EXPECT_EQ(1, entity->GetChildrenCount());
	CEntity* child1 = entity->GetChildByName("test11");
	EXPECT_NE(nullptr, child1);

	EXPECT_EQ(std::string("test11"), child1->GetName());
	EXPECT_TRUE(child1->GetIsInitiallyActive());
	EXPECT_TRUE(child1->IsInitialized());
	EXPECT_TRUE(child1->IsActive());
	EXPECT_FALSE(child1->IsDestroyed());

	EntityParserTestInternal::CCompFoo* component = child1->GetComponent<EntityParserTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);
	EXPECT_TRUE(component->GetIsInitiallyActive());
	EXPECT_TRUE(component->IsInitialized());
	EXPECT_TRUE(component->IsActive());
	EXPECT_FALSE(component->IsDestroyed());
	EXPECT_EQ(1, component->m_a);
	EXPECT_EQ(-3, component->m_b);

	EXPECT_TRUE(child1->HasTags("tag1", "tag3"));
	EXPECT_FALSE(child1->HasTags("tag1", "tag2"));
}

TEST_F(CEntityParserTest, parse_entity_with_child_initiallyActive_false)
{
	CEntityParser parser;
	CEntity* entity = parser.ParseSceneFromJson(EntityParserTestInternal::TWO_LEVEL_ENTITY_INITIALLY_DISABLED);
	EXPECT_NE(nullptr, entity);
	EXPECT_EQ(1, entity->GetChildrenCount());
	CEntity* child1 = entity->GetChildByName("test11");
	EXPECT_NE(nullptr, child1);

	EXPECT_FALSE(child1->GetIsInitiallyActive());
	EXPECT_TRUE(child1->IsInitialized());
	EXPECT_FALSE(child1->IsActive());
	EXPECT_FALSE(child1->IsDestroyed());

	EntityParserTestInternal::CCompFoo* component = child1->GetComponent<EntityParserTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);
	EXPECT_TRUE(component->GetIsInitiallyActive());
	EXPECT_TRUE(component->IsInitialized());
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component->IsDestroyed());
}
