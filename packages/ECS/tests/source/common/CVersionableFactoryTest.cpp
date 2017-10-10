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

#include <utils/CVersionableFactory.h>
#include <utils/CVersionable.h>

#include <gtest/gtest.h>

namespace VersionableFactoryTestInternal
{
	class Foo : public CVersionable
	{
	};
}

class CVersionableFactoryTest : public ::testing::Test
{
public:
	CVersionableFactoryTest()
	{
	}

	~CVersionableFactoryTest()
	{
	}
	CVersionableFactory<VersionableFactoryTestInternal::Foo, 1> m_factory;
};

TEST_F(CVersionableFactoryTest, init_test)
{
	EXPECT_TRUE(true);
}

TEST_F(CVersionableFactoryTest, get_valid_value)
{
	VersionableFactoryTestInternal::Foo* foo1 = m_factory.Get();
	EXPECT_NE(nullptr, foo1);
}

TEST_F(CVersionableFactoryTest, get_invalid_value_if_overflow)
{
	VersionableFactoryTestInternal::Foo* foo1 = m_factory.Get();
	EXPECT_NE(nullptr, foo1);
	VersionableFactoryTestInternal::Foo* foo2 = m_factory.Get();
	EXPECT_EQ(nullptr, foo2);
}

TEST_F(CVersionableFactoryTest, check_value_version)
{
	VersionableFactoryTestInternal::Foo* foo1 = m_factory.Get();
	EXPECT_EQ(0, foo1->GetVersion());
}

TEST_F(CVersionableFactoryTest, check_free)
{
	VersionableFactoryTestInternal::Foo* foo1 = m_factory.Get();
	EXPECT_NE(nullptr, foo1);
	m_factory.Destroy(&foo1);
	EXPECT_EQ(nullptr, foo1);
	VersionableFactoryTestInternal::Foo* foo2 = m_factory.Get();
	EXPECT_NE(nullptr, foo2);
}

TEST_F(CVersionableFactoryTest, check_version_increment)
{
	VersionableFactoryTestInternal::Foo* foo1 = m_factory.Get();
	EXPECT_NE(nullptr, foo1);
	m_factory.Destroy(&foo1);
	EXPECT_EQ(nullptr, foo1);
	VersionableFactoryTestInternal::Foo* foo2 = m_factory.Get();
	EXPECT_NE(nullptr, foo2);
	EXPECT_EQ(1, foo2->GetVersion());
}

TEST_F(CVersionableFactoryTest, check_get_position)
{
	CVersionableFactory<VersionableFactoryTestInternal::Foo, 2> factory;
	VersionableFactoryTestInternal::Foo* foo1 = factory.Get();
	EXPECT_NE(nullptr, foo1);
	int pos1 = factory.GetPosition(foo1);
	EXPECT_EQ(pos1, 0);
	VersionableFactoryTestInternal::Foo* foo2 = factory.Get();
	EXPECT_NE(nullptr, foo2);
	int pos2 = factory.GetPosition(foo2);
	EXPECT_EQ(pos2, 1);
}

TEST_F(CVersionableFactoryTest, check_get_value_with_position_and_version)
{
	VersionableFactoryTestInternal::Foo* foo1 = m_factory.Get();
	EXPECT_NE(nullptr, foo1);
	VersionableFactoryTestInternal::Foo* foo2 = m_factory.GetByIdxAndVersion(0, 0);
	EXPECT_EQ(foo2, foo1);
}

TEST_F(CVersionableFactoryTest, check_get_value_with_position_and_incorrect_version)
{
	VersionableFactoryTestInternal::Foo* foo1 = m_factory.Get();
	EXPECT_NE(nullptr, foo1);
	int version = foo1->GetVersion();
	m_factory.Destroy(&foo1);
	VersionableFactoryTestInternal::Foo* foo2 = m_factory.GetByIdxAndVersion(0, version);
	EXPECT_EQ(nullptr, foo2);
	foo2 = m_factory.Get();
	EXPECT_NE(nullptr, foo2);
	foo2 = m_factory.GetByIdxAndVersion(0, version);
	EXPECT_EQ(nullptr, foo2);
}
