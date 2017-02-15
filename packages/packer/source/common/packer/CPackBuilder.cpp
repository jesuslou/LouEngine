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

#include <packer/CPackBuilder.h>

#include <memory/CFileDataSaver.h>
#include <memory/CMemoryDataProvider.h>
#include <utils/CFilePathHelper.h>
#include <utils/stringUtils.h>

#include <json/reader.h>
#include <json/writer.h>

#include <assert.h>

CPackBuilder::CPackBuilder(const char* globalPropertiesFilePath)
	: m_packerGlobalProperties(globalPropertiesFilePath)
{
	printf("");
}

bool CPackBuilder::GeneratePacksFromConfigurationFile(const char* filePath, const char* singlePackName /*= ""*/)
{
	mSinglePackName = std::string(singlePackName);
	CMemoryDataProvider mdp(filePath);
	if (!mdp.IsValid())
	{
		printf("CPackBuilder::error opening %s!\n", filePath);
		return false;
	}
	return ParseJSONString((const char*) mdp.GetBaseData());
}

bool CPackBuilder::ParseJSONString(const char* str)
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

	Json::Value& packsArray = jsonValue["packs"];
	if (packsArray.type() == Json::arrayValue)
	{
		for (size_t i = 0; i < packsArray.size(); ++i)
		{
			Json::Value& packDefinition = packsArray[i];
			const char* packName = packDefinition["packName"].asCString();
			const bool separatedPacks = packDefinition["separatedPacks"].asBool();
			if (mSinglePackName.empty() || mSinglePackName == packName)
			{
				GeneratePacks(packName, separatedPacks, packDefinition["sourcePaths"]);
			}
		}
		return true;
	}
	return false;
}

void CPackBuilder::GeneratePacks(const char* packName, bool separatedPacks, Json::Value& dataArray)
{
	if (dataArray.type() == Json::arrayValue)
	{
		std::vector<std::string> includedPaths;
		includedPaths.reserve(dataArray.size());
		for (size_t i = 0; i < dataArray.size(); ++i)
		{
			Json::Value& path = dataArray[i];
			includedPaths.emplace_back(path["path"].asString());
		}

		if (separatedPacks)
		{
			GeneratePack(PackerDefinitions::PACK_TYPE_DATA, packName, includedPaths);
			GeneratePack(PackerDefinitions::PACK_TYPE_RESOURCES, packName, includedPaths);
		}
		else
		{
			GeneratePack(PackerDefinitions::PACK_TYPE_ALL, packName, includedPaths);
		}
	}
}

void CPackBuilder::GeneratePack(PackerDefinitions::EPackType packType, const char* packName, const std::vector<std::string>& includedPaths)
{
	const std::vector<CPackerFileExtensionProperties> allowedExtensions = GetAllowedExtensions(packType);

	const char* outputDestinationPath = m_packerGlobalProperties.GetOutputDestinationPath().c_str();
	const char* extension = PackerDefinitions::GetExtensionStr(packType);
	std::string finalPath = StringUtils::Format(outputDestinationPath, "/", packName, extension);
	CFileDataSaver fileDataSaver(finalPath.c_str());

	PackerDefinitions::SPackHeader packHeader;
	packHeader.m_version = m_packerGlobalProperties.GetCurrentVersion();

	fileDataSaver.WritePOD(packHeader);

	for (const std::string& path : includedPaths)
	{
		const CFilePathHelper filePathHelper(path.c_str());
		ProcessFolder(fileDataSaver, path, filePathHelper, allowedExtensions);
	}
}

void CPackBuilder::ProcessFolder(CFileDataSaver& fileDataSaver, const std::string& basePath, const CFilePathHelper& filePathHelper, const std::vector<CPackerFileExtensionProperties>& allowedExtensions)
{
	std::vector<std::string> filesWithAbsolutePath = filePathHelper.GatherAvailableFilesWithAbsolutePath();
	ProcessFiles(fileDataSaver, basePath, filesWithAbsolutePath, allowedExtensions);

	std::vector<std::string> foldersWithAbsolutePath = filePathHelper.GatherAvailableFoldersWithAbsolutePath();
	for (std::string& path : foldersWithAbsolutePath)
	{
		const CFilePathHelper filePathHelper(path.c_str());
		ProcessFolder(fileDataSaver, basePath, filePathHelper, allowedExtensions);
	}
}

void CPackBuilder::ProcessFiles(CFileDataSaver& fileDataSaver, const std::string& basePath, const std::vector<std::string>& filePaths, const std::vector<CPackerFileExtensionProperties>& allowedExtensions)
{
	for (const std::string& path : filePaths)
	{
		std::string fileExtension = CFilePathHelper::getFileExtension(path.c_str());
		if (IsExtensionAllowed(allowedExtensions, fileExtension))
		{
			CMemoryDataProvider memoryDataProvider(path.c_str());
			PackerDefinitions::SPackFileHeader fileHeader;
			std::string fileName(path);
			StringUtils::TrimLeft(fileName, basePath + "/");
			fileHeader.m_nameId = CStrID(fileName.c_str());
			fileHeader.m_nBytes = memoryDataProvider.GetSize();
			fileDataSaver.WritePOD(fileHeader);
			fileDataSaver.Write(memoryDataProvider.GetBufferToWrite(), memoryDataProvider.GetSize());
		}
	}
}

std::vector<CPackerFileExtensionProperties> CPackBuilder::GetAllowedExtensions(PackerDefinitions::EPackType packType) const
{
	std::vector<CPackerFileExtensionProperties> allowedExtensions;
	switch (packType)
	{
		case PackerDefinitions::PACK_TYPE_DATA:
		case PackerDefinitions::PACK_TYPE_RESOURCES:
		allowedExtensions = m_packerGlobalProperties.GetFileProperties(packType);
		break;
		case PackerDefinitions::PACK_TYPE_ALL:
		{
							allowedExtensions = m_packerGlobalProperties.GetFileProperties(PackerDefinitions::PACK_TYPE_DATA);
							const std::vector<CPackerFileExtensionProperties>& resourcesExtensions = m_packerGlobalProperties.GetFileProperties(PackerDefinitions::PACK_TYPE_RESOURCES);
							allowedExtensions.insert(allowedExtensions.end(), resourcesExtensions.begin(), resourcesExtensions.end());
		}
		break;
		default:
		break;
	}
	return allowedExtensions;
}

bool CPackBuilder::IsExtensionAllowed(const std::vector<CPackerFileExtensionProperties>& allowedExtensions, const std::string& extension) const
{
	for (const CPackerFileExtensionProperties& extensionDef : allowedExtensions)
	{
		if (extensionDef.GetExtension() == extension)
		{
			return true;
		}
	}
	return false;
}
