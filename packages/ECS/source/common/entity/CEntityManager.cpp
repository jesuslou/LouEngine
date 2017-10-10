#include <entity/CEntityManager.h>
#include <entity/CEntity.h>

CEntity* CEntityManager::CreateEntity()
{
	return m_entityPool.Get();
}

void CEntityManager::DestroyEntity(CEntity* entity)
{
	m_entityPool.Destroy(&entity);
}

int CEntityManager::GetEntityPosition(CEntity* entity)
{
	return m_entityPool.GetPosition(entity);
}

CEntity* CEntityManager::GetByIdxAndVersion(int index, int version)
{
	return m_entityPool.GetByIdxAndVersion(index, version);
}
