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

#include <hash\CStrID.h>

#include <assert.h>

namespace PackerDefinitions
{
	enum EPackType
	{
		PACK_TYPE_DATA = 0,
		PACK_TYPE_RESOURCES,
		PACK_TYPE_COUNT,
		PACK_TYPE_ALL
	};

	static const char* GetExtensionStr(EPackType packType)
	{
		switch (packType)
		{
			case PACK_TYPE_DATA:
			return ".datapack";
			break;
			case PACK_TYPE_RESOURCES:
			return ".respack";
			break;
			case PACK_TYPE_ALL:
			return ".allpack";
			break;
			default:
			assert(!"Unsupported pack type extension!");
			return "";
			break;
		}
	}

	struct SPackHeader
	{
		static const unsigned VALID_MAGIC = 0xf8a78b;

		unsigned m_magicNumber;
		unsigned m_version;

		SPackHeader() : m_magicNumber(VALID_MAGIC), m_version(0) {}
		bool isValid() const { return m_magicNumber == VALID_MAGIC; }
	};

	struct SPackFileHeader
	{
		CStrID m_nameId;
		unsigned m_nBytes;
	};
}


