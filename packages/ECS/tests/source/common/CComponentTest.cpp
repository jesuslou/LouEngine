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

namespace ComponentTestInternal
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

class CComponentTest : public ::testing::Test
{
public:
	CComponentTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);

		ADD_COMPONENT_FACTORY("foo", ComponentTestInternal::CCompFoo, 1);
	}

	~CComponentTest()
	{
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	CGameSystems m_gameSystems;
	CComponentFactoryManager *m_componentFactoryManager;
};

TEST_F(CComponentTest, component_not_initalized_on_creation)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
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

TEST_F(CComponentTest, component_call_doInit)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	EXPECT_TRUE(component->IsInitialized());
	EXPECT_EQ(1, component->m_initCount);
}

TEST_F(CComponentTest, component_init_called_only_once)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	for (int i = 0; i < ComponentTestInternal::LOOP_COUNT; ++i)
	{
		component->Init();
		EXPECT_TRUE(component->IsInitialized());
		EXPECT_EQ(1, component->m_initCount);
	}
}

TEST_F(CComponentTest, component_call_doActivate)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	component->Init();

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_TRUE(component->IsActive());
	EXPECT_EQ(1, component->m_activateCount);
}

TEST_F(CComponentTest, component_doActivate_called_only_once)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	component->Init();

	for (int i = 0; i < ComponentTestInternal::LOOP_COUNT; ++i)
	{
		component->Activate();
		EXPECT_TRUE(component->IsActive());
		EXPECT_EQ(1, component->m_activateCount);
	}
}

TEST_F(CComponentTest, component_dont_activate_itself_if_uninitialized)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_FALSE(component->IsActive());
	EXPECT_EQ(0, component->m_activateCount);
}

TEST_F(CComponentTest, component_call_doDeactivate)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	component->Init();

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_TRUE(component->IsActive());

	component->Deactivate();
	EXPECT_FALSE(component->IsActive());
	EXPECT_EQ(1, component->m_deactivateCount);
}

TEST_F(CComponentTest, component_doDectivate_called_only_once)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());
	component->Init();

	component->Activate();

	for (int i = 0; i < ComponentTestInternal::LOOP_COUNT; ++i)
	{
		component->Deactivate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_EQ(1, component->m_deactivateCount);
	}
}

TEST_F(CComponentTest, component_initially_disabled_dont_call_doDeactivate)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());
	component->Init();

	for (int i = 0; i < ComponentTestInternal::LOOP_COUNT; ++i)
	{
		component->Deactivate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_EQ(0, component->m_deactivateCount);
	}
}

TEST_F(CComponentTest, component_dont_deactivate_itself_if_uninitialized)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_FALSE(component->IsActive());

	component->Deactivate();
	EXPECT_EQ(0, component->m_deactivateCount);
}

TEST_F(CComponentTest, component_call_doDestroy)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	component->Destroy();
	EXPECT_TRUE(component->IsDestroyed());
	EXPECT_EQ(1, component->m_destroyCount);
}

TEST_F(CComponentTest, component_call_doDestroy_called_only_once)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	
	for (int i = 0; i < ComponentTestInternal::LOOP_COUNT; ++i)
	{
		component->Destroy();
		EXPECT_TRUE(component->IsDestroyed());
		EXPECT_EQ(1, component->m_destroyCount);
	}
}

TEST_F(CComponentTest, component_dont_destroy_itself_if_uninitialized)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Destroy();
	EXPECT_FALSE(component->IsDestroyed());
	EXPECT_EQ(0, component->m_destroyCount);
}

TEST_F(CComponentTest, component_call_doUpdate)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();
	component->Activate();

	for (int i = 1; i < ComponentTestInternal::LOOP_COUNT; ++i)
	{
		component->Update(0.f);
		EXPECT_EQ(i, component->m_updateCount);
	}
}

TEST_F(CComponentTest, component_dont_update_if_inactive)
{
	ComponentTestInternal::CCompFoo* component = static_cast<ComponentTestInternal::CCompFoo*>(
		m_componentFactoryManager->CreateComponent<ComponentTestInternal::CCompFoo>());
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(component->IsInitialized());

	component->Init();

	for (int i = 1; i < ComponentTestInternal::LOOP_COUNT; ++i)
	{
		component->Update(0.f);
		EXPECT_EQ(0, component->m_updateCount);
	}
}
