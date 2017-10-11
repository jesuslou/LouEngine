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

#include <handle/CHandle.h>
#include <entity/CEntity.h>
#include <entity/CEntityManager.h>
#include <component/CComponent.h>
#include <component/CComponentFactoryManager.h>
#include <systems/CSystems.h>

CHandle::CHandle(CEntity* rhs)
{
	*this = rhs ? *rhs : CHandle();
}

CHandle::CHandle(CComponent* rhs)
{
	*this = rhs ? *rhs : CHandle();
}


const CHandle& CHandle::operator=(CEntity* rhs)
{
	*this = rhs ? *rhs : CHandle();
	return *this;
}

const CHandle& CHandle::operator=(CComponent* rhs)
{
	*this = rhs ? *rhs : CHandle();
	return *this;
}

CHandle::operator CEntity*()
{
	return *this ? CSystems::GetSystem<CEntityManager>()->GetElementByIdxAndVersion(m_elementPosition, m_version) : nullptr;
}

CHandle::operator bool()
{
	if (m_elementType == CHandle::EElementType::Entity)
	{
		return CSystems::GetSystem<CEntityManager>()->GetElementByIdxAndVersion(m_elementPosition, m_version) != nullptr;
	}
	else if (m_elementType == CHandle::EElementType::Component)
	{
		return CSystems::GetSystem<CComponentFactoryManager>()->Get(m_componentIdx, m_elementPosition, m_version) != nullptr;
	}
	return false;
}

bool CHandle::operator==(const CHandle& rhs) const
{
	return m_elementType == rhs.m_elementType &&
		m_componentIdx == rhs.m_componentIdx &&
		m_elementPosition == rhs.m_elementPosition &&
		m_version == rhs.m_version;
}
