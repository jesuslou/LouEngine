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

#include <memory/IDataProvider.h>

#include <cassert>

class CMemoryDataProvider : public IDataProvider
{
public:
	using u8 = unsigned char;

	CMemoryDataProvider();
	explicit CMemoryDataProvider(const char* fileName);
	// Memory not owned by this class
	CMemoryDataProvider(const u8* data, std::size_t nBytes);
	// Memory owned by this class
	explicit CMemoryDataProvider(std::size_t nBytes);
	CMemoryDataProvider(const CMemoryDataProvider& mdp);

	void operator =(const CMemoryDataProvider& mdp);

	~CMemoryDataProvider();
	void Destroy();
	bool Load(const char* fileName);
	bool Load(IDataProvider& dp, std::size_t nBytes);

	std::size_t GetSize() const { return m_nBytes; }
	const u8* GetBaseData() const { return m_base; }

	void Read(void* where, std::size_t nBytes) override;
	bool IsValid() const override;
	bool IsOwner() const;

	std::size_t Seek(std::size_t offset, ESeekType from) override;
	std::size_t Tell() const override { return m_currentPosition - m_base; }
	void Rewind();

	std::size_t GetRemainingBytes() const { return GetSize() - (m_currentPosition - m_base); }

	template< class T >
	T* AssignPOD()
	{
		return ConsumeBytes<T>(sizeof(T));
	}

	template< class T >
	T* AssignPODArray(std::size_t nobjects)
	{
		return ConsumeBytes<T>(sizeof(T)*  nobjects);
	}

	const unsigned char* GetTop() const
	{
		return m_currentPosition;
	}

	template< class T >
	T* ConsumeBytes(std::size_t nBytes)
	{
		assert(nBytes <= GetRemainingBytes());
		assert(m_currentPosition + nBytes <= m_base + GetSize());
		T* base = (T*)(m_currentPosition);
		m_currentPosition += nBytes;
		return base;
	}

	const void* GetBufferToWrite() const { return m_base; }

private:
	u8* m_allocatedMemory;
	const u8* m_base;
	std::size_t m_nBytes;
	const u8* m_currentPosition;
};
