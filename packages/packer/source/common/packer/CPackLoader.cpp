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

#include <packer/CPackLoader.h>
#include <memory/CMemoryDataProvider.h>

CPackLoader::CPackLoader(const char* globalPropertiesFilePath, const char* packsPath)
	: m_packerGlobalProperties(globalPropertiesFilePath)
	, m_packsPath(packsPath)
{
}

CPackLoader::CPackLoader(CPackerGlobalProperties& packerGlobalProperties, const char* packsPath)
	: m_packerGlobalProperties(packerGlobalProperties)
	, m_packsPath(packsPath)
{
}

CPack CPackLoader::LoadPack(const char* name, PackerDefinitions::EPackType packType)
{
	CPack generatedPack;

	std::string packExtension = PackerDefinitions::GetExtensionStr(packType);
	std::string finalPath = m_packsPath + "/" + std::string(name) + packExtension;
	CMemoryDataProvider fileDataProvider(finalPath.c_str());
	if (!fileDataProvider.IsValid())
	{
		printf("Pack %s%s doesn't exists in %s folder\n", name, packExtension.c_str(), m_packsPath.c_str());
		return generatedPack;
	}

	PackerDefinitions::SPackHeader packHeader;
	fileDataProvider.ReadPOD(packHeader);
	if (!packHeader.isValid())
	{
		printf("Invalid pack header in %s%s\n", name, packExtension.c_str());
		return generatedPack;
	}
	
	if (packHeader.m_version != m_packerGlobalProperties.GetCurrentVersion())
	{
		printf("Invalid pack version in %s%s. Expected %d. Got %d\n", name, packExtension.c_str(), m_packerGlobalProperties.GetCurrentVersion(), packHeader.m_version);
		return generatedPack;
	}

	while (fileDataProvider.GetRemainingBytes() > sizeof(PackerDefinitions::SPackFileHeader))
	{
		CPackFile packFile;
		PackerDefinitions::SPackFileHeader& fileHeader = packFile.GetHeader();
		fileDataProvider.ReadPOD(fileHeader);
		packFile.InitBuffer();
		fileDataProvider.Read(packFile.GetBuffer(), fileHeader.m_nBytes);
		generatedPack.AddPackFile(packFile);
	}

	return generatedPack;
}
