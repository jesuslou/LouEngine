#pragma once

#include <handle/CHandle.h>
#include <common/CVersionable.h>

class CComponent : public CVersionable
{
	template<class CEntity> friend class CVersionableFactory;
public:
	virtual ~CComponent() {}

	operator CHandle();
	const CComponent* operator=(const CHandle& rhs);
};
