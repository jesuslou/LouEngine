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

#include <memory/CMemoryDataProvider.h>
#include <memory/CFileDataProvider.h>

#include <cassert>

CMemoryDataProvider::CMemoryDataProvider()
	: m_allocatedMemory(nullptr)
	, m_base(nullptr)
	, m_nBytes(0)
	, m_currentPosition(nullptr)
{
}

CMemoryDataProvider::CMemoryDataProvider(const char *filename)
	: m_allocatedMemory(nullptr)
	, m_base(nullptr)
	, m_nBytes(0)
	, m_currentPosition(nullptr)
{
	Load(filename);
}

CMemoryDataProvider::CMemoryDataProvider(const u8* data, std::size_t anbytes)
	: m_allocatedMemory(nullptr)
	, m_base(data)
	, m_nBytes(anbytes)
	, m_currentPosition(data)
{
}

CMemoryDataProvider::CMemoryDataProvider(size_t anbytes)
	: m_nBytes(anbytes)
{
	m_allocatedMemory = static_cast<u8*>(malloc(m_nBytes));
	m_base = m_allocatedMemory;
	m_currentPosition = m_allocatedMemory;
}

CMemoryDataProvider::CMemoryDataProvider(const CMemoryDataProvider &mdp)
	: m_allocatedMemory(nullptr)
	, m_base(nullptr)
	, m_nBytes(0)
	, m_currentPosition(nullptr)
{
	// Use the previous operator to do the checks
	(*this) = mdp;
}

void CMemoryDataProvider::operator =(const CMemoryDataProvider &dp)
{
	// Check sender and receiver is not owner of some buffer, as it's not clear who takes ownership of those
	// buffers. So it's disallowed.
	assert(!dp.m_allocatedMemory);			// Confirm the original is not owner
	assert(!m_allocatedMemory);				// Confirm I'm not owner
	m_allocatedMemory = dp.m_allocatedMemory;
	m_base = dp.m_base;
	m_nBytes = dp.m_nBytes;
	m_currentPosition = dp.m_currentPosition;
	SetName(dp.GetName());
}

bool CMemoryDataProvider::Load(const char *filename)
{

	CFileDataProvider dp(filename);

	if (!dp.IsValid())
	{
		printf("CMemoryDataProvider::load( %s ) : File not found\n", filename);
		assert(dp.IsValid());
	}

	if (!dp.IsValid())
	{
		return false;
	}

	size_t nbytes = CFileDataProvider::GetFileSize(filename);
	return Load(dp, nbytes);
}

bool CMemoryDataProvider::Load(IDataProvider &dp, std::size_t anbytes)
{

	// Free memory if reloading
	if (m_allocatedMemory)
	{
		Destroy();
	}

	SetName(dp.GetName());
	m_nBytes = anbytes;

	if (m_nBytes > 0)
	{
		m_allocatedMemory = static_cast<u8*>(malloc(m_nBytes));
		if (m_allocatedMemory)
		{
			dp.Read(m_allocatedMemory, m_nBytes);
		}
		m_base = m_allocatedMemory;
		m_currentPosition = m_base;
		assert(m_base);
	}
	return true;
}

CMemoryDataProvider::~CMemoryDataProvider()
{
	Destroy();
}

void CMemoryDataProvider::Destroy()
{
	if (m_allocatedMemory)
	{
		free(m_allocatedMemory);
		m_allocatedMemory = nullptr;
	}
	m_nBytes = 0;
	m_base = nullptr;
	m_currentPosition = nullptr;
}

bool CMemoryDataProvider::IsValid() const
{
	return (m_base != nullptr && GetSize() > 0);
}

bool CMemoryDataProvider::IsOwner() const
{
	return m_allocatedMemory != nullptr;
}

void CMemoryDataProvider::Read(void *where, std::size_t nbytes)
{
	if (m_currentPosition + nbytes > m_base + GetSize())
	{
		printf("CMemoryDataProvider::read from dp(%s). Can't read %ld bytes, only %ld/%ld bytes remain at buffer\n",
			   GetName(), nbytes, m_base + GetSize() - m_currentPosition, GetSize());
		assert(m_currentPosition + nbytes <= m_base + GetSize());
	}
	memcpy(where, m_currentPosition, nbytes);
	m_currentPosition += nbytes;
}

std::size_t CMemoryDataProvider::Seek(std::size_t offset, ESeekType whence)
{
	if (whence == ESeekType::FromStart)
	{
		m_currentPosition = m_base + offset;
	}
	else if (whence == ESeekType::FromCurrent)
	{
		m_currentPosition = m_currentPosition + offset;
	}
	else if (whence == ESeekType::FromEndOfFile)
	{
		m_currentPosition = m_base + GetSize() - offset;
	}
	return Tell();
}

void CMemoryDataProvider::Rewind()
{
	m_currentPosition = m_base;
}
