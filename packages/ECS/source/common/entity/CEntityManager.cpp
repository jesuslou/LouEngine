#include <entity/CEntityManager.h>
#include <entity/CEntity.h>

namespace EntityManagerInternal
{
	constexpr int MAX_ENTITIES = 8192;
}

CEntityManager::CEntityManager()
	: m_entityPool(EntityManagerInternal::MAX_ENTITIES)
{
}

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
