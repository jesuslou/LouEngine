#pragma once

#include <utils/CVersionable.h>

#include <cstdio>
#include <cassert>
#include <memory>
#include <vector>

template< typename T>
class CVersionableFactory
{
	static_assert(
		std::is_base_of<CVersionable, T>::value,
		"T must inherits from CVersionable"
		);
public:
	struct SEntry
	{
		T* m_data;
		int m_version;
		bool m_used;
		SEntry() : m_data(nullptr), m_version(0) {}
		SEntry(T* data) : m_data(data), m_version(0), m_used(false) {}
	};

	CVersionableFactory(std::size_t elements)
		: m_n_elements(elements)
	{
		m_entries.resize(m_n_elements);
		m_buffer = malloc(sizeof(T) * m_n_elements);

		T* buffer = (T*) (m_buffer);
		for (SEntry& entry : m_entries)
		{
			entry = SEntry(buffer);
			++buffer;
		}
	}

	~CVersionableFactory()
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

	T* Get()
	{
		for (SEntry& entry : m_entries)
		{
			if (!entry.m_used)
			{
				T* data = new((void*)entry.m_data)T();
				data->SetVersion(entry.m_version);
				entry.m_used = true;
				return data;
			}
		}
		printf("No room for more!\n");
		return nullptr;
	}

	void Destroy(T** data)
	{
		if (*data)
		{
			SEntry *entry = Find(*data);
			if (entry)
			{
				entry->m_data->~T();
				++entry->m_version;
				entry->m_used = false;
				*data = nullptr;
			}
		}
	}

	int GetPosition(T* data)
	{
		if (data)
		{
			for (std::size_t i = 0; i < m_n_elements; ++i)
			{
				if (m_entries[i].m_data == data && data->GetVersion() == m_entries[i].m_version)
				{
					return i;
				}
			}
		}
		return -1;
	}

	T* GetByIdxAndVersion(std::size_t index, int version)
	{
		if (index >= 0 && index < m_n_elements && m_entries[index].m_version == version)
		{
			return m_entries[index].m_data;
		}
		return nullptr;
	}

private:
	void* m_buffer;

	std::size_t m_n_elements;
	std::vector<SEntry> m_entries;

	SEntry* Find(T* data)
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
