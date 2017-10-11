#include <component/CComponent.h>
#include <component/CComponentFactoryManager.h>
#include <systems/CSystems.h>

CComponent::operator CHandle()
{
	return CSystems::GetSystem<CComponentFactoryManager>()->SetHandleInfoFromComponent(this);
}

const CComponent* CComponent::operator=(const CHandle& rhs)
{
	if (rhs.m_elementType == CHandle::EElementType::Component)
	{
		*this = CSystems::GetSystem<CComponentFactoryManager>()->Get(rhs.m_componentIdx, rhs.m_elementPosition, rhs.m_version);
		return this;
	}
	return nullptr;
}
