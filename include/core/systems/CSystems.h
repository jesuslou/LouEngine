#pragma once

#include <core/common/CTypeHasher.h>

#include <unordered_map>

class CSystems
{
public:
	template<typename T>
	static bool SetSystem(T* system, bool forceOverride = false)
	{
		if (GetSystem<T>())
		{
			printf("System of specified type already set\n");
			if (forceOverride)
			{
				DestroySystem<T>();
			}
			else
			{
				return false;
			}
		}
		m_systems[CTypeHasher::Hash<T>()] = system;
		return true;
	}

	template<typename T>
	static T* GetSystem()
	{
		auto systemIt = m_systems.find(CTypeHasher::Hash<T>());
		if (systemIt != m_systems.end())
		{
			return reinterpret_cast<T*>(systemIt->second);
		}
		return nullptr;
	}

	template<typename T>
	static void DestroySystem()
	{
		T* system = GetSystem<T>();
		if (system)
		{
			auto systemIt = m_systems.find(CTypeHasher::Hash<T>());
			if (systemIt != m_systems.end())
			{
				m_systems.erase(systemIt);
			}
			delete system;
		}
	}

protected:
	CSystems() = delete;
	~CSystems() = delete;

	static std::unordered_map<long long, void*>	m_systems;
};

std::unordered_map<long long, void*> CSystems::m_systems;
