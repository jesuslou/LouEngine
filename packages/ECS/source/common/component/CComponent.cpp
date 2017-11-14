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
#include <entity/CEntity.h>
#include <systems/CSystems.h>


CComponent::CComponent()
	: m_numDeactivations(1)
	, m_initialized(false)
	, m_destroyed(false)
	, m_initiallyActive(true)
{
}

CComponent::~CComponent()
{
	for (auto& pair : m_messages)
	{
		delete pair.second;
	}
	m_messages.clear();
}

CComponent::operator CHandle()
{
	return CSystems::GetSystem<CComponentFactoryManager>()->SetHandleInfoFromComponent(this);
}

const CComponent* CComponent::operator=(const CHandle& rhs)
{
	if (rhs.m_elementType == CHandle::EElementType::Component)
	{
		*this = CSystems::GetSystem<CComponentFactoryManager>()->Get(rhs.m_componentIdx, rhs.m_elementPosition, rhs.m_version);
		return this;
	}
	return nullptr;
}

void CComponent::Init()
{
	if (!m_initialized)
	{
		DoInit();
		m_initialized = true;
	}
}

void CComponent::Update(float dt)
{
	if (IsActive())
	{
		DoUpdate(dt);
	}
}

void CComponent::Destroy()
{
	if(m_initialized && !m_destroyed)
	{
		DoDestroy();
		m_destroyed = true;
	}
}

void CComponent::Activate()
{
	if (m_initialized)
	{
		CEntity* owner = m_owner;
		if (m_numDeactivations > 0)
		{
			if (!owner || owner->IsActive())
			{
				m_numDeactivations = 0;
				DoActivate();
			}
			else
			{
				m_numDeactivations = 1;
			}
		}
	}
}

void CComponent::Deactivate()
{
	if (m_initialized)
	{
		if (m_numDeactivations == 0)
		{
			DoDeactivate();
			++m_numDeactivations;
		}
	}
}

void CComponent::ActivateFromParent()
{
	if (m_numDeactivations > 0)
	{
		--m_numDeactivations;
		if (m_numDeactivations == 0)
		{
			DoActivate();
		}
	}
}

void CComponent::CheckFirstActivation()
{
	if (m_initialized)
	{
		if (GetIsInitiallyActive())
		{
			CheckFirstActivationInternal();
		}
		else
		{
			++m_numDeactivations;
		}
	}
}

void CComponent::CheckFirstActivationInternal()
{
	CEntity* owner = m_owner;
	if ((!owner || owner->IsActive()) && m_numDeactivations > 0)
	{
		--m_numDeactivations;
	}
}
