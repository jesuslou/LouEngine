#include <entity/CEntity.h>
#include <entity/CEntityManager.h>
#include <systems/CSystems.h>

CEntity::operator CHandle()
{
	CHandle handle;

	handle.m_elementType = 1;
	handle.m_componentIdx = 0;
	handle.m_elementPosition = CSystems::GetSystem<CEntityManager>()->GetEntityPosition(this);
	handle.m_version = GetVersion();

	return handle;
}

const CEntity* CEntity::operator=(const CHandle& rhs)
{
	if (rhs.m_elementType == 1)
	{
		*this = CSystems::GetSystem<CEntityManager>()->GetByIdxAndVersion(rhs.m_elementPosition, rhs.m_version);
		return this;
	}
	return nullptr;
}
