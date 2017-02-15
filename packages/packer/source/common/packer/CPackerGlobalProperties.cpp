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

#include <packer/CPackerGlobalProperties.h>
#include <memory/CMemoryDataProvider.h>

#include <json/reader.h>

#include <assert.h>

CPackerGlobalProperties::CPackerGlobalProperties(const char* filePath)
	: m_version(0)
{
	LoadFromJSON(filePath);
}

bool CPackerGlobalProperties::LoadFromJSON(const char* filePath)
{
	CMemoryDataProvider mdp(filePath);
	if (!mdp.IsValid())
	{
		printf("PackerGlobalProperties::error opening %s!\n", filePath);
		return false;
	}
	return ParseJSONString((const char*) mdp.GetBaseData());
}

const std::vector<CPackerFileExtensionProperties>& CPackerGlobalProperties::GetFileProperties(PackerDefinitions::EPackType packType) const
{
	assert(packType < PackerDefinitions::PACK_TYPE_COUNT);
	return mFileProperties[packType];
}

bool CPackerGlobalProperties::ParseJSONString(const char* str)
{
	Json::Value jsonValue;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(str, jsonValue);
	if (!parsingSuccessful)
	{
		std::string error = reader.getFormattedErrorMessages();
		printf("PackerGlobalProperties::Error processing Json: %s\n", error.c_str());
		return false;
	}

	m_outputDestinationPath = std::string(jsonValue["outputDestinationPath"].asCString());
	m_version = jsonValue["version"].asInt();

	ParsePackerFileTypes(PackerDefinitions::PACK_TYPE_DATA, jsonValue["dataPackFileTypes"]);
	ParsePackerFileTypes(PackerDefinitions::PACK_TYPE_RESOURCES, jsonValue["resourcePackFileTypes"]);

	return true;
}

void CPackerGlobalProperties::ParsePackerFileTypes(PackerDefinitions::EPackType packType, Json::Value& jsonArray)
{
	if (jsonArray.type() == Json::arrayValue)
	{
		for (size_t i = 0; i < jsonArray.size(); ++i)
		{
			Json::Value& extensionDef = jsonArray[i];
			const char* extension = extensionDef["extension"].asCString();
			const char* rule = extensionDef["rule"].asCString();
			mFileProperties[packType].emplace_back(CPackerFileExtensionProperties{extension, rule});
		}
	}
}
