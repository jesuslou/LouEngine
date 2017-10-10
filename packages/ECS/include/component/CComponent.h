#pragma once

#include <handle/CHandle.h>
#include <utils/CVersionable.h>

class CComponent : public CVersionable
{
	template<class CEntity> friend class CVersionableFactory;
public:
	operator CHandle();

	const CComponent* operator=(const CHandle& rhs);
	CComponent()
	{}
	~CComponent()
	{}
};
