#pragma once

#include <cstdio>
#include <cassert>
#include <memory>

template< class T, size_t N>
class CMemoryPool
{
public:
	struct SEntry
	{
		T* m_data;
		SEntry *m_next;
		int m_version;
		SEntry() : m_data(nullptr), m_next(nullptr), m_version(-1) {}
	};

	CMemoryPool()
	{
		m_buffer = malloc(sizeof(T) * N);

		T* t_buffer = (T*) (m_buffer);
		m_entries[0].m_data = t_buffer;
		++t_buffer;
		for (size_t i = 1; i < N; ++i)
		{
			m_entries[i].m_data = t_buffer;
			m_entries[i - 1].m_next = &m_entries[i];
			++t_buffer;
		}

		m_current = &m_entries[0];
	}

	~CMemoryPool()
	{
		delete m_buffer;
	}

	SEntry* GetWithExtraData()
	{
		if (m_current)
		{
			T* data = new((void*) m_current->m_data)T();
			++m_current->m_version;
			SEntry* temp = m_current;
			m_current = m_current->m_next;
			return temp;
		}
		else
		{
			printf("No room for more!\n");
		}
		return nullptr;
	}

	T* Get()
	{
		if (m_current)
		{
			T* data = new((void*)m_current->m_data)T();
			++m_current->m_version;
			m_current = m_current->m_next;
			return data;
		}
		else
		{
			printf("No room for more!\n");
		}
		return nullptr;
	}

	void Destroy(T** data)
	{
		SEntry *entry = Find(*data);
		if (entry)
		{
			entry->m_data->~T();
			entry->m_next = m_current;
			m_current = entry;
			*data = nullptr;
		}
	}

private:
	void* m_buffer;
	SEntry* m_current;

	SEntry m_entries[N];

	SEntry* Find(T* data)
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (m_entries[i].m_data == data)
			{
				return &m_entries[i];
			}
		}
		return nullptr;
	}
};
