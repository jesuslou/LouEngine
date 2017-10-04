#include <LouEnginePrecompile.h>

#include <entities/CEntityManager.h>

CEntity CEntityManager::CreateNewEntity()
{
	return m_entityX.entities.create();
}

CEntity CEntityManager::CloneEntity(CEntity baseEntity)
{
	return m_entityX.entities.create_from_copy(baseEntity);
}
