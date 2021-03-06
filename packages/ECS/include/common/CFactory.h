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

#include <common/CECSElement.h>

#include <cstdio>
#include <memory>
#include <vector>

template< typename T>
class CFactory
{
	static_assert(
		std::is_base_of<CECSElement, T>::value,
		"T must inherits from CECSElement if you want to use a CFactory"
		);
public:
	CFactory(std::size_t numElements)
		: m_numElements(numElements)
	{
		m_entries.resize(m_numElements);
		m_buffer = malloc(sizeof(T) * m_numElements);

		T* buffer = static_cast<T*>(m_buffer);
		m_entries[0].m_data = buffer;
		++buffer;
		for (size_t i = 1; i < m_numElements; ++i)
		{
			m_entries[i].m_data = buffer;
			m_entries[i - 1].m_next = &m_entries[i];
			++buffer;
		}
		m_current = &m_entries[0];
	}

	virtual ~CFactory()
	{
		for (SEntry& entry : m_entries)
		{
			if (entry.m_used)
			{
				entry.m_data->~T();
			}
		}
		delete m_buffer;
	}

	T* GetNewElement()
	{
		if (m_current)
		{
			T* data = new(static_cast<void*>(m_current->m_data))T();
			data->SetVersion(m_current->m_version);
			m_current->m_used = true;
			m_current = m_current->m_next;
			return data;
		}
		printf("No room for more!\n");
		return nullptr;
	}

	bool DestroyElement(T** data)
	{
		if (*data)
		{
			SEntry *entry = FindElement(*data);
			if (entry)
			{
				entry->m_data->~T();
				++entry->m_version;
				entry->m_next = m_current;
				entry->m_used = false;
				m_current = entry;
				*data = nullptr;
				return true;
			}
		}
		return false;
	}

	int GetPositionForElement(T* data)
	{
		if (data)
		{
			for (std::size_t i = 0; i < m_numElements; ++i)
			{
				if (m_entries[i].m_data == data && data->GetVersion() == m_entries[i].m_version)
				{
					return i;
				}
			}
		}
		return -1;
	}

	T* GetElementByIdxAndVersion(std::size_t index, int version)
	{
		if (index >= 0 && index < m_numElements && m_entries[index].m_version == version)
		{
			return m_entries[index].m_data;
		}
		return nullptr;
	}

protected:
	struct SEntry
	{
		T* m_data;
		SEntry *m_next;
		int m_version;
		bool m_used;
		SEntry() : m_data(nullptr), m_next(nullptr), m_version(0), m_used(false) {}
	};

	void* m_buffer;

	std::size_t m_numElements;
	std::vector<SEntry> m_entries;
	SEntry* m_current;

	SEntry* FindElement(T* data)
	{
		for (SEntry& entry : m_entries)
		{
			if (entry.m_data == data)
			{
				return &entry;
			}
		}
		return nullptr;
	}
};
