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

#include <tags/CTagsManager.h>
#include <systems/CSystems.h>

#include <gtest/gtest.h>

class CTagsTest : public ::testing::Test
{
public:
	CTagsTest()
		: m_tagManager(new CTagsManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CTagsManager>(m_tagManager);
	}

	~CTagsTest()
	{
		m_gameSystems.DestroySystem<CTagsManager>();
	}

	CGameSystems m_gameSystems;
	CTagsManager *m_tagManager;
};

TEST_F(CTagsTest, register_new_tag)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
}

TEST_F(CTagsTest, register_existing_tag_fails)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
	success = m_tagManager->RegisterTag("test");
	EXPECT_FALSE(success);
}

TEST_F(CTagsTest, register_tag_correct_value)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
	success = m_tagManager->RegisterTag("test2");
	EXPECT_TRUE(success);
	int tagId1 = m_tagManager->GetTagIdx("test");
	EXPECT_EQ(0, tagId1);
	int tagId2 = m_tagManager->GetTagIdx("test2");
	EXPECT_EQ(1, tagId2);
}

TEST_F(CTagsTest, check_add_tag)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);

	TagsMask mask;
	success = m_tagManager->SetTag(mask, "test", true);
	EXPECT_TRUE(success);
	success = m_tagManager->SetTag(mask, "test2", true);
	EXPECT_FALSE(success);

	success = m_tagManager->HasTag(mask, "test");
	EXPECT_TRUE(success);
	success = m_tagManager->HasTag(mask, "test2");
	EXPECT_FALSE(success);
}

TEST_F(CTagsTest, check_remove_tag)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);

	TagsMask mask;
	success = m_tagManager->SetTag(mask, "test", true);
	EXPECT_TRUE(success);
	success = m_tagManager->HasTag(mask, "test");
	EXPECT_TRUE(success);

	success = m_tagManager->SetTag(mask, "test", false);
	EXPECT_TRUE(success);
	success = m_tagManager->HasTag(mask, "test");
	EXPECT_FALSE(success);
}

TEST_F(CTagsTest, check_add_tags_in_bunch)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
	success = m_tagManager->RegisterTag("test2");
	EXPECT_TRUE(success);

	TagsMask mask;
	success = m_tagManager->AddTags(mask, "test", "test2");
	EXPECT_TRUE(success);

	success = m_tagManager->HasTag(mask, "test");
	EXPECT_TRUE(success);
	success = m_tagManager->HasTag(mask, "test2");
	EXPECT_TRUE(success);
}

TEST_F(CTagsTest, check_add_tags_in_bunch_with_incorrect_one)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);

	TagsMask mask;
	success = m_tagManager->AddTags(mask, "test3", "test", "test2");
	EXPECT_FALSE(success);

	success = m_tagManager->HasTag(mask, "test");
	EXPECT_TRUE(success);
	success = m_tagManager->HasTag(mask, "test2");
	EXPECT_FALSE(success);
}

TEST_F(CTagsTest, check_remove_tags_in_bunch)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
	success = m_tagManager->RegisterTag("test2");
	EXPECT_TRUE(success);

	TagsMask mask;
	success = m_tagManager->AddTags(mask, "test", "test2");
	EXPECT_TRUE(success);

	success = m_tagManager->HasTag(mask, "test");
	EXPECT_TRUE(success);
	success = m_tagManager->HasTag(mask, "test2");
	EXPECT_TRUE(success);

	success = m_tagManager->RemoveTags(mask, "test", "test2");
	EXPECT_TRUE(success);

	success = m_tagManager->HasTag(mask, "test");
	EXPECT_FALSE(success);
	success = m_tagManager->HasTag(mask, "test2");
	EXPECT_FALSE(success);
}

TEST_F(CTagsTest, check_remove_tags_in_bunch_with_incorrect_one)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);

	TagsMask mask;
	success = m_tagManager->AddTags(mask, "test");
	EXPECT_TRUE(success);

	success = m_tagManager->HasTag(mask, "test");
	EXPECT_TRUE(success);

	success = m_tagManager->RemoveTags(mask, "test3", "test", "test2");
	EXPECT_FALSE(success);

	success = m_tagManager->HasTag(mask, "test");
	EXPECT_FALSE(success);
}

TEST_F(CTagsTest, check_generate_tag_mask)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
	success = m_tagManager->RegisterTag("test2");
	EXPECT_TRUE(success);

	int tagId1 = m_tagManager->GetTagIdx("test");
	EXPECT_EQ(0, tagId1);
	int tagId2 = m_tagManager->GetTagIdx("test2");
	EXPECT_EQ(1, tagId2);

	TagsMask mask;
	mask[tagId1] = true;
	TagsMask newMask = m_tagManager->GetTagMask("test");
	EXPECT_EQ(mask, newMask);
	newMask = m_tagManager->GetTagMask("test", "test2");
	EXPECT_NE(mask, newMask);
	mask[tagId2] = true;
	EXPECT_EQ(mask, newMask);
}

TEST_F(CTagsTest, check_nonexistent_tag)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);

	int tagId1 = m_tagManager->GetTagIdx("test");
	EXPECT_EQ(0, tagId1);
	int tagId2 = m_tagManager->GetTagIdx("test2");
	EXPECT_EQ(-1, tagId2);

	TagsMask mask;
	mask[tagId1].flip();
	EXPECT_TRUE(m_tagManager->HasTag(mask, "test"));
	EXPECT_FALSE(m_tagManager->HasTag(mask, "test2"));
}

TEST_F(CTagsTest, generate_mask_with_nonexistent_tag)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);

	TagsMask mask = m_tagManager->GetTagMask("test", "test2");
	EXPECT_TRUE(m_tagManager->HasTag(mask, "test"));
	EXPECT_FALSE(m_tagManager->HasTag(mask, "test2"));
}

TEST_F(CTagsTest, check_has_all_tags)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
	success = m_tagManager->RegisterTag("test2");
	EXPECT_TRUE(success);

	int tagId1 = m_tagManager->GetTagIdx("test");
	EXPECT_EQ(0, tagId1);
	int tagId2 = m_tagManager->GetTagIdx("test2");
	EXPECT_EQ(1, tagId2);

	TagsMask mask;
	mask[tagId1] = true;
	success = m_tagManager->HasAllTags(mask, "test");
	EXPECT_TRUE(success);
	success = m_tagManager->HasAllTags(mask, "test", "test2");
	EXPECT_FALSE(success);
	mask[tagId2] = true;
	success = m_tagManager->HasAllTags(mask, "test");
	EXPECT_TRUE(success);
	success = m_tagManager->HasAllTags(mask, "test", "test2");
	EXPECT_TRUE(success);
}

TEST_F(CTagsTest, check_has_any_tags)
{
	bool success = m_tagManager->RegisterTag("test");
	EXPECT_TRUE(success);
	success = m_tagManager->RegisterTag("test2");
	EXPECT_TRUE(success);

	int tagId1 = m_tagManager->GetTagIdx("test");
	EXPECT_EQ(0, tagId1);
	int tagId2 = m_tagManager->GetTagIdx("test2");
	EXPECT_EQ(1, tagId2);

	TagsMask mask;
	mask[tagId1] = true;
	success = m_tagManager->HasAnyTags(mask, "test");
	EXPECT_TRUE(success);
	success = m_tagManager->HasAnyTags(mask, "test", "test2");
	EXPECT_TRUE(success);
	TagsMask mask2;
	mask2[tagId2] = true;
	success = m_tagManager->HasAnyTags(mask2, "test");
	EXPECT_FALSE(success);
	success = m_tagManager->HasAnyTags(mask2, "test", "test2");
	EXPECT_TRUE(success);
}
