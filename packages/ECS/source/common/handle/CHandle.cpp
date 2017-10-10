#include <handle/CHandle.h>
#include <entity/CEntity.h>
#include <entity/CEntityManager.h>
#include <systems/CSystems.h>

CHandle::CHandle(CEntity* rhs)
{
	if (rhs)
	{
		*this = *rhs;
	}
	else
	{
		*this = CHandle();
	}
}

const CHandle& CHandle::operator=(CEntity* rhs)
{
	*this = rhs ? *rhs : CHandle();
	return *this;
}

CHandle::operator CEntity*()
{
	if (*this)
	{
		return CSystems::GetSystem<CEntityManager>()->GetByIdxAndVersion(m_elementPosition, m_version);
	}
	return nullptr;
}

CHandle::operator bool()
{
	if (m_elementType == 1)
	{
		return CSystems::GetSystem<CEntityManager>()->GetByIdxAndVersion(m_elementPosition, m_version) != nullptr;
	}
	return false;
}
