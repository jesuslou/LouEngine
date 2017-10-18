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

namespace EntityComponentTestInternal
{
	const int LOOP_COUNT = 5;

	class CCompFoo : public CComponent
	{};

	class CCompBar : public CComponent
	{};
}

class CEntityComponentTest : public ::testing::Test
{
public:
	CEntityComponentTest()
		: m_componentFactoryManager(new CComponentFactoryManager)
		, m_entityManager(new CEntityManager)
	{
		CSystems::SetGameSystems(&m_gameSystems);
		m_gameSystems.SetSystem<CComponentFactoryManager>(m_componentFactoryManager);
		m_gameSystems.SetSystem<CEntityManager>(m_entityManager);

		ADD_COMPONENT_FACTORY("foo", EntityComponentTestInternal::CCompFoo, 10);
		ADD_COMPONENT_FACTORY("bar", EntityComponentTestInternal::CCompBar, 1);
	}

	~CEntityComponentTest()
	{
		m_gameSystems.DestroySystem<CEntityManager>();
		m_gameSystems.DestroySystem<CComponentFactoryManager>();
	}

	std::tuple<CEntity*, CEntity*, CComponent*, CComponent*> GetEntityWithChildren()
	{
		CEntity* entity = m_entityManager->GetNewElement();
		EXPECT_NE(nullptr, entity);
		CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
		EXPECT_NE(nullptr, component);
		CEntity* entity1 = m_entityManager->GetNewElement();
		EXPECT_NE(nullptr, entity1);
		CComponent* component1 = entity1->AddComponent<EntityComponentTestInternal::CCompFoo>();
		EXPECT_NE(nullptr, component1);

		entity->AddChild(entity1);

		return std::make_tuple(entity, entity1, component, component1);
	}

	std::tuple<CEntity*, CEntity*, CEntity*, CComponent*, CComponent*, CComponent*> GetEntityWithChildrenRecursive()
	{
		CEntity* entity = m_entityManager->GetNewElement();
		EXPECT_NE(nullptr, entity);
		CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
		EXPECT_NE(nullptr, component);
		CEntity* entity1 = m_entityManager->GetNewElement();
		EXPECT_NE(nullptr, entity1);
		CComponent* component1 = entity1->AddComponent<EntityComponentTestInternal::CCompFoo>();
		EXPECT_NE(nullptr, component1);
		CEntity* entity11 = m_entityManager->GetNewElement();
		EXPECT_NE(nullptr, entity11);
		CComponent* component11 = entity11->AddComponent<EntityComponentTestInternal::CCompFoo>();
		EXPECT_NE(nullptr, component11);

		entity->AddChild(entity1);
		entity1->AddChild(entity11);

		return std::make_tuple(entity, entity1, entity11, component, component1, component11);
	}

	CGameSystems m_gameSystems;
	CEntityManager *m_entityManager;
	CComponentFactoryManager *m_componentFactoryManager;
};

TEST_F(CEntityComponentTest, add_component_by_class)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);
	EntityComponentTestInternal::CCompFoo* foo = dynamic_cast<EntityComponentTestInternal::CCompFoo*>(component);
	EXPECT_NE(nullptr, foo);
	EntityComponentTestInternal::CCompBar* bar = dynamic_cast<EntityComponentTestInternal::CCompBar*>(component);
	EXPECT_EQ(nullptr, bar);

	CHandle handle = entity->AddComponent("bar");
	EXPECT_TRUE(static_cast<bool>(handle));
	component = handle;
	bar = dynamic_cast<EntityComponentTestInternal::CCompBar*>(component);
	EXPECT_NE(nullptr, bar);
	foo = dynamic_cast<EntityComponentTestInternal::CCompFoo*>(component);
	EXPECT_EQ(nullptr, foo);
}

TEST_F(CEntityComponentTest, add_component_by_name)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent("foo");
	EXPECT_NE(nullptr, component);
	EntityComponentTestInternal::CCompFoo* foo = dynamic_cast<EntityComponentTestInternal::CCompFoo*>(component);
	EXPECT_NE(nullptr, foo);
	EntityComponentTestInternal::CCompBar* bar = dynamic_cast<EntityComponentTestInternal::CCompBar*>(component);
	EXPECT_EQ(nullptr, bar);

	CHandle handle = entity->AddComponent("bar");
	EXPECT_TRUE(static_cast<bool>(handle));
	component = handle;
	bar = dynamic_cast<EntityComponentTestInternal::CCompBar*>(component);
	EXPECT_NE(nullptr, bar);
	foo = dynamic_cast<EntityComponentTestInternal::CCompFoo*>(component);
	EXPECT_EQ(nullptr, foo);
}

TEST_F(CEntityComponentTest, get_component_by_class)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	CComponent* component = entity->GetComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);
	EntityComponentTestInternal::CCompFoo* foo = dynamic_cast<EntityComponentTestInternal::CCompFoo*>(component);
	EXPECT_NE(nullptr, foo);
	EntityComponentTestInternal::CCompBar* bar = dynamic_cast<EntityComponentTestInternal::CCompBar*>(component);
	EXPECT_EQ(nullptr, bar);
}

TEST_F(CEntityComponentTest, get_component_by_name)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	CComponent* component = entity->GetComponent("foo");
	EXPECT_NE(nullptr, component);
	EntityComponentTestInternal::CCompFoo* foo = dynamic_cast<EntityComponentTestInternal::CCompFoo*>(component);
	EXPECT_NE(nullptr, foo);
	EntityComponentTestInternal::CCompBar* bar = dynamic_cast<EntityComponentTestInternal::CCompBar*>(component);
	EXPECT_EQ(nullptr, bar);
}

TEST_F(CEntityComponentTest, remove_component_by_class)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	CHandle handle = entity->GetComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_TRUE(static_cast<bool>(handle));

	bool success = entity->RemoveComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_TRUE(success);

	CComponent* component = entity->GetComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_EQ(nullptr, component);
	EXPECT_FALSE(static_cast<bool>(handle));
}

TEST_F(CEntityComponentTest, remove_component_by_name)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	CHandle handle = entity->GetComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_TRUE(static_cast<bool>(handle));

	bool success = entity->RemoveComponent("foo");
	EXPECT_TRUE(success);

	CComponent* component = entity->GetComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_EQ(nullptr, component);
	EXPECT_FALSE(static_cast<bool>(handle));
}

TEST_F(CEntityComponentTest, check_cant_add_component_twice)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CHandle handle = entity->AddComponent<EntityComponentTestInternal::CCompBar>();
	EXPECT_TRUE(static_cast<bool>(handle));
	handle = entity->AddComponent<EntityComponentTestInternal::CCompBar>();
	EXPECT_FALSE(static_cast<bool>(handle));
}

TEST_F(CEntityComponentTest, check_cant_remove_component_twice)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CHandle handle = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_TRUE(static_cast<bool>(handle));

	bool success = entity->RemoveComponent("foo");
	EXPECT_TRUE(success);
	success = entity->RemoveComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_FALSE(success);
}

TEST_F(CEntityComponentTest, check_component_invalid_after_deleting_entity)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CHandle handle = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_TRUE(static_cast<bool>(handle));

	m_entityManager->DestroyEntity(&entity);
	EXPECT_EQ(nullptr, entity);

	EXPECT_FALSE(static_cast<bool>(handle));
	CComponent* component = handle;
	EXPECT_EQ(nullptr, component);
}

TEST_F(CEntityComponentTest, check_component_owner)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	CHandle entityHandle = entity;
	EXPECT_TRUE(static_cast<bool>(entityHandle));
	CHandle ownerHandle = component->GetOwner();

	EXPECT_EQ(ownerHandle, entityHandle);
}

TEST_F(CEntityComponentTest, init_entity_initialize_components)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	EXPECT_FALSE(entity->IsInitialized());
	EXPECT_FALSE(component->IsInitialized());
	entity->Init();
	EXPECT_TRUE(entity->IsInitialized());
	EXPECT_TRUE(component->IsInitialized());
}

TEST_F(CEntityComponentTest, destroy_entity_destroys_components)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CHandle compHandle = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_TRUE(static_cast<bool>(compHandle));

	EXPECT_FALSE(entity->IsDestroyed());
	entity->Destroy();
	EXPECT_TRUE(entity->IsDestroyed());
	EXPECT_FALSE(static_cast<bool>(compHandle));
}

TEST_F(CEntityComponentTest, activate_entity_activates_components)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);
	entity->Init();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	entity->Activate();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(component->IsActive());
}

TEST_F(CEntityComponentTest, entity_activate_activates_components_children)
{
	CEntity* entity, *entity1 = nullptr;
	CComponent* component, *component1 = nullptr;
	std::tie(entity, entity1, component, component1) = GetEntityWithChildren();

	entity->Init();
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	entity->Activate();
	EXPECT_TRUE(component->IsActive());
	EXPECT_TRUE(component1->IsActive());
}

TEST_F(CEntityComponentTest, entity_activate_activates_components_children_recursive)
{
	CEntity* entity, *entity1, *entity11 = nullptr;
	CComponent* component, *component1, *component11 = nullptr;
	std::tie(entity, entity1, entity11, component, component1, component11) = GetEntityWithChildrenRecursive();

	entity->Init();
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	EXPECT_FALSE(component11->IsActive());
	entity->Activate();
	EXPECT_TRUE(component->IsActive());
	EXPECT_TRUE(component1->IsActive());
	EXPECT_TRUE(component11->IsActive());
}

TEST_F(CEntityComponentTest, deactivate_entity_deactivates_components)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	entity->Activate();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(component->IsActive());

	entity->Deactivate();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
}

TEST_F(CEntityComponentTest, deactivate_entity_deactivates_components_children)
{
	CEntity* entity, *entity1 = nullptr;
	CComponent* component, *component1 = nullptr;
	std::tie(entity, entity1, component, component1) = GetEntityWithChildren();

	entity->Init();

	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	entity->Activate();
	EXPECT_TRUE(component->IsActive());
	EXPECT_TRUE(component1->IsActive());

	entity->Deactivate();
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
}

TEST_F(CEntityComponentTest, deactivate_entity_deactivates_components_children_recursive)
{
	CEntity* entity, *entity1, *entity11 = nullptr;
	CComponent* component, *component1, *component11 = nullptr;
	std::tie(entity, entity1, entity11, component, component1, component11) = GetEntityWithChildrenRecursive();

	entity->Init();

	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	EXPECT_FALSE(component11->IsActive());
	entity->Activate();
	EXPECT_TRUE(component->IsActive());
	EXPECT_TRUE(component1->IsActive());
	EXPECT_TRUE(component11->IsActive());

	entity->Deactivate();
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	EXPECT_FALSE(component11->IsActive());
}

TEST_F(CEntityComponentTest, entity_and_component_initiallyActive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	entity->SetIsInitiallyActive(true);
	component->SetIsInitiallyActive(true);
	entity->CheckFirstActivation();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(component->IsActive());
}

TEST_F(CEntityComponentTest, entity_and_component_initiallyActive_children)
{
	CEntity* entity, *entity1 = nullptr;
	CComponent* component, *component1 = nullptr;
	std::tie(entity, entity1, component, component1) = GetEntityWithChildren();

	entity->Init();

	entity->SetIsInitiallyActive(true);
	entity1->SetIsInitiallyActive(true);
	component->SetIsInitiallyActive(true);
	component1->SetIsInitiallyActive(true);
	entity->CheckFirstActivation();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(entity1->IsActive());
	EXPECT_TRUE(component->IsActive());
	EXPECT_TRUE(component1->IsActive());
}

TEST_F(CEntityComponentTest, entity_and_component_initiallyActive_children_recursive)
{
	CEntity* entity, *entity1, *entity11 = nullptr;
	CComponent* component, *component1, *component11 = nullptr;
	std::tie(entity, entity1, entity11, component, component1, component11) = GetEntityWithChildrenRecursive();

	entity->Init();

	entity->SetIsInitiallyActive(true);
	entity1->SetIsInitiallyActive(true);
	entity11->SetIsInitiallyActive(true);
	component->SetIsInitiallyActive(true);
	component1->SetIsInitiallyActive(true);
	component11->SetIsInitiallyActive(true);
	entity->CheckFirstActivation();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(entity1->IsActive());
	EXPECT_TRUE(entity11->IsActive());
	EXPECT_TRUE(component->IsActive());
	EXPECT_TRUE(component1->IsActive());
	EXPECT_TRUE(component11->IsActive());
}

TEST_F(CEntityComponentTest, entity_active_and_component_deactivated_initiallyActive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	entity->SetIsInitiallyActive(true);
	component->SetIsInitiallyActive(false);
	entity->CheckFirstActivation();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
}

TEST_F(CEntityComponentTest, entity_active_and_component_deactivated_initiallyActive_children)
{
	CEntity* entity, *entity1 = nullptr;
	CComponent* component, *component1 = nullptr;
	std::tie(entity, entity1, component, component1) = GetEntityWithChildren();

	entity->Init();

	component->SetIsInitiallyActive(false);
	component1->SetIsInitiallyActive(false);
	entity->CheckFirstActivation();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(entity1->IsActive());
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());

	for (int i = 0; i < EntityComponentTestInternal::LOOP_COUNT; ++i)
	{
		entity->Activate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_FALSE(component1->IsActive());
	}
	for (int i = 0; i < EntityComponentTestInternal::LOOP_COUNT; ++i)
	{
		entity1->Activate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_FALSE(component1->IsActive());
	}
}

TEST_F(CEntityComponentTest, entity_active_and_component_deactivated_initiallyActive_children_recursive)
{
	CEntity* entity, *entity1, *entity11 = nullptr;
	CComponent* component, *component1, *component11 = nullptr;
	std::tie(entity, entity1, entity11, component, component1, component11) = GetEntityWithChildrenRecursive();

	entity->Init();

	component->SetIsInitiallyActive(false);
	component1->SetIsInitiallyActive(false);
	component11->SetIsInitiallyActive(false);
	entity->CheckFirstActivation();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(entity1->IsActive());
	EXPECT_TRUE(entity11->IsActive());
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	EXPECT_FALSE(component11->IsActive());

	for (int i = 0; i < EntityComponentTestInternal::LOOP_COUNT; ++i)
	{
		entity->Activate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_FALSE(component1->IsActive());
		EXPECT_FALSE(component11->IsActive());
	}
	for (int i = 0; i < EntityComponentTestInternal::LOOP_COUNT; ++i)
	{
		entity1->Activate();
		EXPECT_FALSE(component->IsActive());
		EXPECT_FALSE(component1->IsActive());
		EXPECT_FALSE(component11->IsActive());
	}
}

TEST_F(CEntityComponentTest, component_no_active_despite_initiallyActive_if_entity_not_initiallyActive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	entity->SetIsInitiallyActive(false);
	component->SetIsInitiallyActive(true);
	entity->CheckFirstActivation();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
}

TEST_F(CEntityComponentTest, component_no_active_despite_initiallyActive_if_entity_not_initiallyActive_children)
{
	CEntity* entity, *entity1 = nullptr;
	CComponent* component, *component1 = nullptr;
	std::tie(entity, entity1, component, component1) = GetEntityWithChildren();

	entity->Init();

	entity->SetIsInitiallyActive(false);
	component->SetIsInitiallyActive(true);
	component1->SetIsInitiallyActive(true);
	entity->CheckFirstActivation();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	entity->Activate();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(component->IsActive());
	EXPECT_TRUE(component1->IsActive());
}

TEST_F(CEntityComponentTest, component_no_active_despite_initiallyActive_if_entity_not_initiallyActive_children_recursive)
{
	CEntity* entity, *entity1, *entity11 = nullptr;
	CComponent* component, *component1, *component11 = nullptr;
	std::tie(entity, entity1, entity11, component, component1, component11) = GetEntityWithChildrenRecursive();

	entity->Init();

	entity->SetIsInitiallyActive(true);
	entity1->SetIsInitiallyActive(false);
	entity11->SetIsInitiallyActive(false);
	entity->CheckFirstActivation();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_FALSE(entity1->IsActive());
	EXPECT_FALSE(entity11->IsActive());
	EXPECT_TRUE(component->IsActive());
	EXPECT_FALSE(component1->IsActive());
	EXPECT_FALSE(component11->IsActive());
	for (int i = 0; i < EntityComponentTestInternal::LOOP_COUNT; ++i)
	{
		entity1->Activate();
		EXPECT_TRUE(entity1->IsActive());
		EXPECT_FALSE(entity11->IsActive());
		EXPECT_TRUE(component1->IsActive());
		EXPECT_FALSE(component11->IsActive());
	}
	for (int i = 0; i < EntityComponentTestInternal::LOOP_COUNT; ++i)
	{
		entity11->Activate();
		EXPECT_TRUE(entity11->IsActive());
		EXPECT_TRUE(component11->IsActive());
	}
}

TEST_F(CEntityComponentTest, inactive_entity_activates_component_initiallyActive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	entity->SetIsInitiallyActive(false);
	component->SetIsInitiallyActive(true);
	entity->CheckFirstActivation();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	entity->Activate();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(component->IsActive());
}

TEST_F(CEntityComponentTest, inactive_entity_dont_activates_inactive_component_initiallyActive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	entity->SetIsInitiallyActive(false);
	component->SetIsInitiallyActive(false);
	entity->CheckFirstActivation();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	entity->Activate();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
}

TEST_F(CEntityComponentTest, component_inactive_after_activation_if_entity_inactive)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	entity->SetIsInitiallyActive(false);
	component->SetIsInitiallyActive(false);
	entity->CheckFirstActivation();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
}

TEST_F(CEntityComponentTest, component_inactive_after_activation_if_entity_inactive_2)
{
	CEntity* entity = m_entityManager->GetNewElement();
	EXPECT_NE(nullptr, entity);
	CComponent* component = entity->AddComponent<EntityComponentTestInternal::CCompFoo>();
	EXPECT_NE(nullptr, component);

	entity->Init();

	entity->SetIsInitiallyActive(false);
	component->SetIsInitiallyActive(false);
	entity->CheckFirstActivation();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	component->Activate();
	EXPECT_FALSE(entity->IsActive());
	EXPECT_FALSE(component->IsActive());
	entity->Activate();
	EXPECT_TRUE(entity->IsActive());
	EXPECT_TRUE(component->IsActive());
}
