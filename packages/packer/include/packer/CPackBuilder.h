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

#include <packer/CPackerGlobalProperties.h>
#include <packer/CPackerFileExtensionProperties.h>

namespace Json
{
	class Value;
}

class CFileDataSaver;
class CFilePathHelper;

class CPackBuilder
{
public:
	CPackBuilder() {}
	CPackBuilder(const char* globalPropertiesFilePath);

	bool GeneratePacksFromConfigurationFile(const char* filePath);
	bool GeneratePacksFromConfigurationFile(const char* filePath, const std::vector<std::string>& specificPacks);
private:
	bool ParseJSONString(const char* str);

	void GeneratePacks(const char* packName, bool separatedPacks, Json::Value& dataArray);
	void GeneratePack(PackerDefinitions::EPackType packType, const char* packName, const std::vector<std::string>& includedPaths);

	void ProcessFolder(CFileDataSaver& fileDataSaver, const std::string& basePath, const CFilePathHelper& filePathHelper, const std::vector<CPackerFileExtensionProperties>& allowedExtensions);
	void ProcessFiles(CFileDataSaver& fileDataSaver, const std::string& basePath, const std::vector<std::string>& filePaths, const std::vector<CPackerFileExtensionProperties>& allowedExtensions);

	std::vector<CPackerFileExtensionProperties> GetAllowedExtensions(PackerDefinitions::EPackType packType) const;
	bool IsExtensionAllowed(const std::vector<CPackerFileExtensionProperties>& allowedExtensions, const std::string& extension) const;

	CPackerGlobalProperties m_packerGlobalProperties;
	std::string m_outputDestinationPath;
	std::vector<std::string> m_specificPacks;
};
