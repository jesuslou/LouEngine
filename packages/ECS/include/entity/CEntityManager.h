#pragma once

#include <common/CFactory.h>
#include <entity/CEntity.h>

class CEntityManager : public CFactory<CEntity>
{
	friend class CEntity;
public:
	CEntityManager();
	~CEntityManager() override {}
private:
};
