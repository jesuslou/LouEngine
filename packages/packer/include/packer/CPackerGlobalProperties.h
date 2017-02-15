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

#include <packer/PackerDefinitions.h>
#include <packer/CPackerFileExtensionProperties.h>

#include <vector>

namespace Json
{
	class Value;
}

class CPackerGlobalProperties
{
public:
	CPackerGlobalProperties() : m_version(0) {}
	CPackerGlobalProperties(const char* filePath);

	bool LoadFromJSON(const char* filePath);

	const std::vector<CPackerFileExtensionProperties>& GetFileProperties(PackerDefinitions::EPackType packType) const;

	const std::string& GetOutputDestinationPath() const { return m_outputDestinationPath; }
	int GetCurrentVersion() const { return m_version; }

private:
	bool ParseJSONString(const char* str);
	void ParsePackerFileTypes(PackerDefinitions::EPackType packType, Json::Value& jsonArray);

	std::string m_outputDestinationPath;
	int m_version;
	std::vector<CPackerFileExtensionProperties> mFileProperties[PackerDefinitions::PACK_TYPE_COUNT];
};
