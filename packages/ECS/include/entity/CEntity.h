#pragma once

#include <handle/CHandle.h>
#include <common/CVersionable.h>

class CEntity : public CVersionable
{
	template<class CEntity> friend class CFactory;
public:
	operator CHandle();

	const CEntity* operator=(const CHandle& rhs);
private:
	CEntity()
	{}
	~CEntity()
	{}
};
