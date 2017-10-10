#pragma once

#include <handle/CHandle.h>
#include <utils/CVersionable.h>

class CEntity : public CVersionable
{
	template<class CEntity, size_t> friend class CVersionableFactory;
public:
	operator CHandle();

	const CEntity* operator=(const CHandle& rhs);
private:
	CEntity()
	{}
	~CEntity()
	{}
};
