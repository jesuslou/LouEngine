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

#include <entity/CEntityManager.h>
#include <entity/CEntityParser.h>
#include <component/CComponent.h>
#include <memory/CMemoryDataProvider.h>
#include <systems/CSystems.h>
#include <hash/CStrID.h>
#include <component/CComponentFactoryManager.h>

#include <json/reader.h>

CEntityParser::CEntityParser()
	: m_entityManager(*CSystems::GetSystem<CEntityManager>())
{
}

CHandle CEntityParser::ParseScene(const char* const path)
{
	CMemoryDataProvider mdp(path);
	if (!mdp.IsValid())
	{
		printf("CEntityParser::error opening %s!\n", path);
		return CHandle();
	}

	return ParseSceneFromJson((const char*)mdp.GetBaseData());
}

CHandle CEntityParser::ParseSceneFromJson(const char* const jsonStr)
{
	CHandle result;
	Json::Value jsonValue;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(jsonStr, jsonValue);
	if (!parsingSuccessful)
	{
		std::string error = reader.getFormattedErrorMessages();
		printf("CEntityParser::Error processing Json: %s\n", error.c_str());
		return result;
	}

	Json::Value& rootEntity = jsonValue["root"];
	if (rootEntity.type() == Json::objectValue)
	{
		if (rootEntity["type"].asString() == "entity")
		{
			result = ParseEntity(rootEntity, nullptr);
		}
		else if (rootEntity["type"].asString() == "prefab")
		{
			result = ParsePrefab(rootEntity);
		}
	}

	return result;
}

CHandle CEntityParser::ParseEntity(Json::Value& entityData, CEntity* parent)
{
	CEntity* entity = m_entityManager.GetNewElement();
	entity->SetName(entityData["name"].asCString());
	
	ParseTags(entityData["tags"], entity);
	ParseComponents(entityData["components"], entity);
	ParseChildren(entityData["children"], entity);

	if (parent)
	{
		parent->AddChild(entity);
	}
	return entity;
}

CHandle CEntityParser::ParsePrefab(Json::Value& entityData)
{
	return CHandle();
}

bool CEntityParser::ParseTags(Json::Value& tags, CEntity* entity)
{
	if (tags.type() == Json::arrayValue)
	{
		for (size_t i = 0; i < tags.size(); ++i)
		{
			Json::Value& tag = tags[i];
			entity->AddTags(tag.asCString());
		}
		return true;
	}
	return false;
}

bool CEntityParser::ParseComponents(Json::Value& components, CEntity* entity)
{
	if (components.type() == Json::arrayValue)
	{
		for (size_t i = 0; i < components.size(); ++i)
		{
			Json::Value& componentJson = components[i];
			CComponent* component = entity->AddComponent(CStrID(componentJson["type"].asCString()));
			if (component)
			{
				component->ParseAtts(componentJson);
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool CEntityParser::ParseChildren(Json::Value& children, CEntity* entity)
{
	if (children.type() == Json::arrayValue)
	{
		for (size_t i = 0; i < children.size(); ++i)
		{
			Json::Value& childJson = children[i];
			ParseEntity(childJson, entity);
		}
		return true;
	}
	return false;
}
