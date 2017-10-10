#pragma once

#include <utils/CVersionableFactory.h>
#include <entity/CEntity.h>

class CEntityManager
{
	friend class CEntity;
public:
	CEntityManager();

	CEntity* CreateEntity();

	void DestroyEntity(CEntity* entity);
	CEntity* GetByIdxAndVersion(int index, int version);
	int GetEntityPosition(CEntity* entity);

private:
	CVersionableFactory<CEntity> m_entityPool;
};
