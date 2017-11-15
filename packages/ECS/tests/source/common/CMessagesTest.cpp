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

	class CCompFoo : public CComponent
	{
	public:
		CCompFoo()
			: m_initCount(0)
			, m_updateCount(0)
			, m_destroyCount(0)
			, m_activateCount(0)
			, m_deactivateCount(0)
		{}

		void DoInit() override { ++m_initCount; }
		void DoUpdate(float /*dt*/) override { ++m_updateCount; }
		void DoDestroy() override { ++m_destroyCount; }
		void DoActivate() override { ++m_activateCount; }
		void DoDeactivate() override { ++m_deactivateCount; }

		int m_initCount;
		int m_updateCount;
		int m_destroyCount;
		int m_activateCount;
		int m_deactivateCount;
	};
}

class CMessagesTest : public ::testing::Test
{
public:
	CMessagesTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);

		ADD_COMPONENT_FACTORY("foo", MessagesTestInternal::CCompFoo, 1);
	}

	~CMessagesTest()
	{
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CComponentFactoryManager *m_componentFactoryManager;
};

TEST_F(CMessagesTest, component_not_initalized_on_creation)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());
	EXPECT_FALSE(component->IsDestroyed());
	EXPECT_FALSE(component->IsActive());

	EXPECT_EQ(0, component->m_initCount);
	EXPECT_EQ(0, component->m_destroyCount);
	EXPECT_EQ(0, component->m_updateCount);
	EXPECT_EQ(0, component->m_activateCount);
	EXPECT_EQ(0, component->m_deactivateCount);
}

TEST_F(CMessagesTest, component_call_doInit)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	EXPECT_TRUE(component->IsInitialized());
	EXPECT_EQ(1, component->m_initCount);
}

TEST_F(CMessagesTest, component_init_called_only_once)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	for (int i = 0; i < MessagesTestInternal::LOOP_COUNT; ++i)
	{
		component->Init();
		EXPECT_TRUE(component->IsInitialized());
		EXPECT_EQ(1, component->m_initCount);
	}
}

TEST_F(CMessagesTest, component_call_doActivate)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	component->Init();

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_TRUE(component->IsActive());
	EXPECT_EQ(1, component->m_activateCount);
}

TEST_F(CMessagesTest, component_doActivate_called_only_once)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	component->Init();

	for (int i = 0; i < MessagesTestInternal::LOOP_COUNT; ++i)
	{
		component->Activate();
		EXPECT_TRUE(component->IsActive());
		EXPECT_EQ(1, component->m_activateCount);
	}
}

TEST_F(CMessagesTest, component_dont_activate_itself_if_uninitialized)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_FALSE(component->IsActive());
	EXPECT_EQ(0, component->m_activateCount);
}

TEST_F(CMessagesTest, component_call_doDeactivate)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	component->Init();

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_TRUE(component->IsActive());

	component->Deactivate();
	EXPECT_FALSE(component->IsActive());
	EXPECT_EQ(1, component->m_deactivateCount);
}

TEST_F(CMessagesTest, component_doDectivate_called_only_once)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());
	component->Init();

	component->Activate();

	for (int i = 0; i < MessagesTestInternal::LOOP_COUNT; ++i)
	{
		component->Deactivate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_EQ(1, component->m_deactivateCount);
	}
}

TEST_F(CMessagesTest, component_initially_disabled_dont_call_doDeactivate)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());
	component->Init();

	for (int i = 0; i < MessagesTestInternal::LOOP_COUNT; ++i)
	{
		component->Deactivate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_EQ(0, component->m_deactivateCount);
	}
}

TEST_F(CMessagesTest, component_dont_deactivate_itself_if_uninitialized)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_FALSE(component->IsActive());

	component->Deactivate();
	EXPECT_EQ(0, component->m_deactivateCount);
}

TEST_F(CMessagesTest, component_call_doDestroy)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	component->Destroy();
	EXPECT_TRUE(component->IsDestroyed());
	EXPECT_EQ(1, component->m_destroyCount);
}

TEST_F(CMessagesTest, component_call_doDestroy_called_only_once)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	
	for (int i = 0; i < MessagesTestInternal::LOOP_COUNT; ++i)
	{
		component->Destroy();
		EXPECT_TRUE(component->IsDestroyed());
		EXPECT_EQ(1, component->m_destroyCount);
	}
}

TEST_F(CMessagesTest, component_dont_destroy_itself_if_uninitialized)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Destroy();
	EXPECT_FALSE(component->IsDestroyed());
	EXPECT_EQ(0, component->m_destroyCount);
}

TEST_F(CMessagesTest, component_call_doUpdate)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	component->Activate();

	for (int i = 1; i < MessagesTestInternal::LOOP_COUNT; ++i)
	{
		component->Update(0.f);
		EXPECT_EQ(i, component->m_updateCount);
	}
}

TEST_F(CMessagesTest, component_dont_update_if_inactive)
{
	MessagesTestInternal::CCompFoo* component = static_cast<MessagesTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<MessagesTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();

	for (int i = 1; i < MessagesTestInternal::LOOP_COUNT; ++i)
	{
		component->Update(0.f);
		EXPECT_EQ(0, component->m_updateCount);
	}
}
