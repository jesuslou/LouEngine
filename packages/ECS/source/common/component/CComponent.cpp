#include <component/CComponent.h>
#include <systems/CSystems.h>

CComponent::operator CHandle()
{
	CHandle handle;

	handle.m_elementType = 1;
	handle.m_componentIdx = 0;
	//handle.m_elementPosition = CSystems::GetSystem<CComponentManager>()->GetEntityPosition(this);
	handle.m_version = GetVersion();

	return handle;
}

const CComponent* CComponent::operator=(const CHandle& rhs)
{
	if (rhs.m_elementType == 1)
	{
		//*this = CSystems::GetSystem<CComponentManager>()->GetByIdxAndVersion(rhs.m_elementPosition, rhs.m_version);
		return this;
	}
	return nullptr;
}
