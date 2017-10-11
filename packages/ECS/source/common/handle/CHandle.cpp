#include <handle/CHandle.h>
#include <entity/CEntity.h>
#include <entity/CEntityManager.h>
#include <component/CComponent.h>
#include <component/CComponentFactoryManager.h>
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

CHandle::CHandle(CComponent* rhs)
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

const CHandle& CHandle::operator=(CComponent* rhs)
{
	*this = rhs ? *rhs : CHandle();
	return *this;
}

CHandle::operator CEntity*()
{
	if (*this)
	{
		return CSystems::GetSystem<CEntityManager>()->GetElementByIdxAndVersion(m_elementPosition, m_version);
	}
	return nullptr;
}

CHandle::operator bool()
{
	if (m_elementType == CHandle::EElementType::Entity)
	{
		return CSystems::GetSystem<CEntityManager>()->GetElementByIdxAndVersion(m_elementPosition, m_version) != nullptr;
	}
	else if (m_elementType == CHandle::EElementType::Component)
	{
		return CSystems::GetSystem<CComponentFactoryManager>()->Get(m_componentIdx, m_elementPosition, m_version) != nullptr;
	}
	return false;
}
