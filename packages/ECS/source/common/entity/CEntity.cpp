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
#include <systems/CSystems.h>

CEntity::operator CHandle()
{
	CHandle handle;

	handle.m_elementType = CHandle::EElementType::Entity;
	handle.m_componentIdx = 0;
	handle.m_elementPosition = CSystems::GetSystem<CEntityManager>()->GetPositionForElement(this);
	handle.m_version = GetVersion();

	return handle;
}

const CEntity* CEntity::operator=(const CHandle& rhs)
{
	if (rhs.m_elementType == CHandle::EElementType::Entity)
	{
		*this = CSystems::GetSystem<CEntityManager>()->GetElementByIdxAndVersion(rhs.m_elementPosition, rhs.m_version);
		return this;
	}
	return nullptr;
}
