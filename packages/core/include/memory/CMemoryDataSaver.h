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

#include "memory/IDataSaver.h"

class CMemoryDataSaver : public IDataSaver
{
public:
	using u8 = unsigned char;

	CMemoryDataSaver();
	CMemoryDataSaver(std::size_t requiredBytes, std::size_t initialSize = 0);
	CMemoryDataSaver(unsigned char *buffer, std::size_t bufferSize);
	~CMemoryDataSaver();

	void operator =(const CMemoryDataSaver& mds);

	void Alloc(std::size_t required_bytes, std::size_t initial_size = 0);
	void Destroy();

	void Rewind(std::size_t initial_size = 0);

	void Write(const void *data, std::size_t data_bytes) override;
	bool IsValid() const override { return m_base != nullptr; }
	bool IsOwner() const { return m_isOwnerOfBase; }

	unsigned char *GetBuffer() { return m_base; }
	const unsigned char *GetBuffer() const { return m_base; }
	unsigned char *GetTop() const { return m_top; }

	std::size_t GetCapacity() const { return m_reservedBytes; }
	std::size_t GetSize() const { return m_top - m_base; }
	std::size_t GetRemainingBytes() const { return GetCapacity() - GetSize(); }
	bool IsEmpty() const { return m_top == m_base; }
	void Resize(std::size_t new_size);

	template< class T >
	void PushBack(const T &obj)
	{
		Write(&obj, sizeof(T));
	}

	void WriteLong(int v)
	{
		PushBack(v);
	}

	template< class TObj >
	TObj *ConsumePOD()
	{
		assert(GetSize() + sizeof(TObj) <= GetCapacity()/* || sys_fatal("CMemoryDataSaver::consumeBytes( %d ) failed. Size:%ld Capacity:%ld\n", nbytes, GetSize(), GetCapacity())*/);
		TObj *prev_top = (TObj *) m_top;
		m_top += sizeof(TObj);
		return prev_top;
	}

	template< class TObj >
	TObj *ConsumeArray(std::size_t nobjs)
	{
		assert(GetSize() + (sizeof(TObj) *nobjs) <= GetCapacity() /*|| sys_fatal("CMemoryDataSaver::consumeArray( %d (objs) ) failed. nObjs:%ld Capacity:%ld BytesPerObj:%ld\n", nobjs, GetSize(), GetCapacity(), sizeof(TObj))*/);
		TObj *prev_top = (TObj *) m_top;
		m_top += sizeof(TObj) * nobjs;
		return prev_top;
	}

	const void *GetBufferToWrite() const { return m_base; }
	std::size_t getNumBytesToWrite() const { return GetSize(); }

private:
	u8 *m_base;
	u8 *m_top;
	std::size_t m_reservedBytes;
	bool m_isOwnerOfBase;
};
