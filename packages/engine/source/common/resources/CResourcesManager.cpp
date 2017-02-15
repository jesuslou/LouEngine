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

#include <resources/CResourcesManager.h>
#include <packer/CPackLoader.h>

CResourcesManager::CResourcesManager(const char* globalPropertiesFilePath, const char* packsPath)
	: m_packerGlobalProperties(globalPropertiesFilePath)
	, m_packsPath(packsPath)
{
}

bool CResourcesManager::LoadPack(const char* name, PackerDefinitions::EPackType packType)
{
	CPackLoader packLoader(m_packerGlobalProperties, m_packsPath.c_str());
	CPack newPack = packLoader.LoadPack(name, packType);
	if (!newPack.IsValid())
	{
		if (packType == PackerDefinitions::PACK_TYPE_ALL)
		{
			bool result = false;
			result |= LoadPack(name, PackerDefinitions::PACK_TYPE_DATA);
			result |= LoadPack(name, PackerDefinitions::PACK_TYPE_RESOURCES);
			return result;
		}
		return false;
	}

	m_packs.emplace_back(std::move(newPack));
	return true;
}
