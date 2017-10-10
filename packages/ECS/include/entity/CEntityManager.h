#pragma once

#include <memory/CMemoryPool.h>
#include <utils/CVersionableFactory.h>
#include <entity/CEntity.h>

class CEntityManager
{
	friend class CEntity;
public:
	CEntity* CreateEntity();

	void DestroyEntity(CEntity* entity);
	CEntity* GetByIdxAndVersion(int index, int version);
	int GetEntityPosition(CEntity* entity);

private:
	static constexpr int MAX_ENTITIES = 2;// 8192;
	CVersionableFactory<CEntity, MAX_ENTITIES> m_entityPool;
};
