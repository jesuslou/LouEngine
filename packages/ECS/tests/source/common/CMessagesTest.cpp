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

#include <component/CComponent.h>
#include <component/CComponentFactoryManager.h>
#include <entity/CEntityManager.h>
#include <handle/CHandle.h>
#include <systems/CSystems.h>

#include <gtest/gtest.h>


//*************************
//*************************
//TO DO
// Check component receives message
// Check component receives (or not. Decide first) message before initialization
// Check two different components in same entity receives same message
// Check component doesn't receives message if deactivated
// Check component doesn't receives message if destroyed
// Check components in different entities receives a broadcast message
//*************************
//*************************

namespace MessagesTestInternal
{
	const int LOOP_COUNT = 5;

	struct STestMessage
	{
		STestMessage(int dummy) : m_dummy(dummy) {}
		int m_dummy;
	};

	struct STestMessage2
	{
	};

	class CCompFoo : public CComponent
	{
	public:
		CCompFoo()
			: m_foo(0)
		{}

		void RegisterMessages() override
		{
			RegisterMessage(&CCompFoo::OnTestMessage);
		}

		void OnTestMessage(const STestMessage& message)
		{
			m_foo = message.m_dummy;
		}

		int m_foo;
	};

	class CCompBar : public CComponent
	{
	public:
		CCompBar()
			: m_bar(0)
		{}

		void RegisterMessages() override
		{
			RegisterMessage(&CCompBar::OnTestMessage);
			RegisterMessage(&CCompBar::OnTestMessage2);
		}

		void OnTestMessage(const STestMessage& message)
		{
			m_bar = message.m_dummy * 2;
		}

		void OnTestMessage2(const STestMessage2& /*message*/)
		{
			m_bar = 1337;
		}

		int m_bar;
	};
}

class CMessagesTest : public ::testing::Test
{
public:
	CMessagesTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
		, m_entityManager(new CEntityManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);

		ADD_COMPONENT_FACTORY("foo", MessagesTestInternal::CCompFoo, 2);
		ADD_COMPONENT_FACTORY("bar", MessagesTestInternal::CCompBar, 2);
	}

	~CMessagesTest()
	{
		m_gameSystems.DestroySystem<CEntityManager>();
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CComponentFactoryManager *m_componentFactoryManager;
	CEntityManager *m_entityManager;
};

TEST_F(CMessagesTest, component_receives_registered_sendMessage)
{
	MessagesTestInternal::CCompFoo* compFoo = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, compFoo);

	compFoo->Init();
	compFoo->Activate();
	EXPECT_EQ(0, compFoo->m_foo);
	MessagesTestInternal::STestMessage message(10);
	compFoo->SendMessage(message);
	EXPECT_EQ(10, compFoo->m_foo);
}

TEST_F(CMessagesTest, component_ignores_unregistered_sendMessage)
{
	MessagesTestInternal::CCompFoo* compFoo = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, compFoo);

	compFoo->Init();
	compFoo->Activate();
	EXPECT_EQ(0, compFoo->m_foo);
	MessagesTestInternal::STestMessage2 message;
	compFoo->SendMessage(message);
	EXPECT_EQ(0, compFoo->m_foo);
}

TEST_F(CMessagesTest, entity_with_component_receives_message)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	MessagesTestInternal::CCompFoo* compFoo = entity->AddComponent<MessagesTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, compFoo);
}
