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

#pragma once

#include <common/CFactory.h>
#include <entity/CEntity.h>

class CEntity;
class CEntityManager;
class CPrefabManager;

namespace Json
{
	class Value;
}

class CEntityParser
{
public:
	CEntityParser();

	CHandle ParseScene(const char* const path);
	CHandle ParseSceneFromJson(const char* const jsonStr);

private:
	CHandle ParseEntity(Json::Value& entityData, CEntity* parent);
	CHandle ParsePrefab(Json::Value& entityData);

	bool ParseTags(Json::Value& tags, CEntity* entity);
	bool ParseComponents(Json::Value& components, CEntity* entity);
	bool ParseChildren(Json::Value& children, CEntity* entity);

	CEntityManager& m_entityManager;
	CPrefabManager& m_prefabManager;
};
