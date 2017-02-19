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

#include <memory/CFileDataProvider.h>

#include <cassert>

namespace CFileDataProviderInternal
{
	static const std::size_t INFINITE_SIZE = (~(0));
}

CFileDataProvider::CFileDataProvider()
	: m_startOffset(0)
	, m_maxBytes(CFileDataProviderInternal::INFINITE_SIZE)
{
}

CFileDataProvider::CFileDataProvider(const char* fileName)
	: m_startOffset(0)
	, m_maxBytes(CFileDataProviderInternal::INFINITE_SIZE)
{
	Open(fileName);
}

CFileDataProvider::~CFileDataProvider()
{
	m_startOffset = 0;
	m_maxBytes = CFileDataProviderInternal::INFINITE_SIZE;
	Close();
}

std::size_t CFileDataProvider::GetFileSize(const char* fileName)
{
	CFileDataProvider dataProvider(fileName);
	if (!dataProvider.IsValid())
	{
		return 0;
	}
	std::size_t nBytes = dataProvider.Seek(0, ESeekType::FromEndOfFile);
	return nBytes;
}

std::size_t CFileDataProvider::GetSize() const
{
	if (!m_file)
	{
		return 0;
	}
	std::size_t size = 0;
	fseek(m_file, 0L, SEEK_END);
	size = ftell(m_file);
	fseek(m_file, 0L, SEEK_SET);
	return size;
}

bool CFileDataProvider::FileExist(const char* fileName)
{
	FILE* file = fopen(fileName, "rb");
	if (file)
	{
		fclose(file);
		return true;
	}
	return false;
}

bool CFileDataProvider::Open(const char* fileName)
{
	SetName(fileName);
	m_file = fopen(fileName, "rb");
	if (!IsValid())
	{
		printf("Failed to open read file %s\n", fileName);
	}
	return IsValid();
}

bool CFileDataProvider::IsValid() const
{
	return m_file != nullptr;
}

void CFileDataProvider::Close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
}

void CFileDataProvider::Read(void* where, std::size_t nBytes)
{
	assert(IsValid());
	size_t nBytesRead = fread(where, 1, nBytes, m_file);
	assert(nBytesRead == nBytes);
}

std::size_t CFileDataProvider::Seek(std::size_t offset, ESeekType from)
{
	assert(IsValid());
	int rc = fseek(m_file, static_cast<long>(offset), static_cast<int>(from));
	assert(rc == 0);
	int newPosition = ftell(m_file);
	assert(from != ESeekType::FromStart || newPosition == offset);
	return newPosition;
}

std::size_t CFileDataProvider::Tell() const
{
	return ftell(m_file);
}
