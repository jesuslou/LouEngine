#pragma once

#include <utils/CVersionable.h>

#include <cstdio>
#include <cassert>
#include <memory>

template< typename T, size_t N>
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
		int m_pos;
		int m_version;
		bool m_used;
		SEntry() : m_data(nullptr), m_pos(0), m_version(0) {}
		SEntry(T* data, int pos) : m_data(data), m_pos(pos), m_version(0), m_used(false) {}
	};

	CVersionableFactory()
	{
		m_buffer = malloc(sizeof(T) * N);

		T* buffer = (T*) (m_buffer);
		for (size_t i = 0; i < N; ++i)
		{
			m_entries[i] = SEntry(buffer, i);
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
		SEntry *entry = Find(*data);
		if (entry)
		{
			entry->m_data->~T();
			++entry->m_version;
			entry->m_used = false;
			*data = nullptr;
		}
	}

	int GetPosition(T* data)
	{
		for (int i = 0; i < N; ++i)
		{
			if (m_entries[i].m_data == data && data->GetVersion() == m_entries[i].m_version)
			{
				return i;
			}
		}
		return -1;
	}

	T* GetByIdxAndVersion(int index, int version)
	{
		if (index >= 0 && index < N && m_entries[index].m_version == version)
		{
			return m_entries[index].m_data;
		}
		return nullptr;
	}

private:
	void* m_buffer;

	SEntry m_entries[N];

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
