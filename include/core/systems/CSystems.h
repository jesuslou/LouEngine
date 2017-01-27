#pragma once

#include <core/common/CTypeHasher.h>

#include <unordered_map>

class CGameSystems
{
public:

	~CGameSystems()
	{
		for (auto& system : m_systems)
		{
			delete system.second;
		}
		m_systems.clear();
	}

	template<typename T>
	bool SetSystem(T* system, bool forceOverride = false)
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
	T* GetSystem()
	{
		auto systemIt = m_systems.find(CTypeHasher::Hash<T>());
		if (systemIt != m_systems.end())
		{
			return reinterpret_cast<T*>(systemIt->second);
		}
		return nullptr;
	}

	template<typename T>
	void DestroySystem()
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
	std::unordered_map<long long, void*> m_systems;
};

class CSystems
{
public:
	static void SetGameSystems(CGameSystems* gameSystems) { s_gameSystems = gameSystems; }

	template<class T>
	static void SetSystem(T* system, bool forceOverride = false) { s_gameSystems->SetSystem<T>(system, forceOverride); }
	template<class T>
	static T* GetSystem() { return s_gameSystems->GetSystem<T>(); }
	template<class T>
	static void DestroySystem() { return s_gameSystems->DestroySystem<T>(); }
private:
	static CGameSystems* s_gameSystems;
};
