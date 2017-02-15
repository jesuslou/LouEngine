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

#include <LouEnginePrecompile.h>

#include <utils/CTypeHasher.h>

#include <unordered_map>

class CGameSystems
{
public:

	~CGameSystems()
	{
		for (auto& system : m_systems)
		{
			DELETE_POINTER(system.second);
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
			DELETE_POINTER(system);
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
