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

#include <memory/CMemoryDataSaver.h>

#include <cassert>

CMemoryDataSaver::CMemoryDataSaver()
	: m_base(nullptr)
	, m_top(nullptr)
	, m_reservedBytes(0)
	, m_isOwnerOfBase(false)
{
}

CMemoryDataSaver::CMemoryDataSaver(size_t requiredBytes, std::size_t initialSize) 
	: m_base(nullptr)
	, m_top(nullptr)
	, m_reservedBytes(0)
{
	Alloc(requiredBytes, initialSize);
}

CMemoryDataSaver::CMemoryDataSaver(unsigned char *buffer, std::size_t bufferSize)
	: m_base(buffer)
	, m_top(buffer)
	, m_reservedBytes(bufferSize)
	, m_isOwnerOfBase(false)
{
}

CMemoryDataSaver::~CMemoryDataSaver()
{
	Destroy();
}

void CMemoryDataSaver::operator= (const CMemoryDataSaver &ds)
{
	if (m_isOwnerOfBase)
	{
		assert(m_base);
		assert(m_reservedBytes >= ds.m_reservedBytes);
		memcpy(m_base, ds.m_base, ds.GetSize());
		Rewind(ds.GetSize());
	}
	else
	{
		assert((!ds.m_isOwnerOfBase && !m_isOwnerOfBase) && "Can't operator copy of CMemoryDataSaver..., destructor class, free owned buffer!\n");
		m_base = ds.m_base;
		m_top = ds.m_top;
		m_reservedBytes = ds.m_reservedBytes;
		m_isOwnerOfBase = ds.m_isOwnerOfBase;
	}
}

void CMemoryDataSaver::Destroy()
{
	if (m_base && m_isOwnerOfBase)
	{
		free(m_base);
	}
	m_base = nullptr;
	m_top = nullptr;
	m_reservedBytes = 0;
	m_isOwnerOfBase = false;
}

void CMemoryDataSaver::Rewind(size_t initial_size/* = 0*/)
{
	assert(m_base); m_top = m_base + initial_size;
}

void CMemoryDataSaver::Alloc(size_t requiredBytes, std::size_t initialSize)
{
	Destroy();
	m_isOwnerOfBase = true;
	m_reservedBytes = requiredBytes;
	m_base = (u8 *) malloc(m_reservedBytes);
	m_top = m_base + initialSize;
}

void CMemoryDataSaver::Write(const void *data, std::size_t dataBytes)
{
	assert(IsValid());
	assert(GetSize() + dataBytes <= m_reservedBytes && "Can't write %ld bytes. Current size is %ld. Reserved size is %ld\n");
	memcpy(m_top, data, dataBytes);
	m_top += dataBytes;
}

void CMemoryDataSaver::Resize(size_t newSize)
{
	assert(newSize <= m_reservedBytes);
	m_top = m_base + newSize;
}