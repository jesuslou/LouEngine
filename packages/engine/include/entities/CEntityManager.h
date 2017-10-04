#pragma once

#include <entities/EntityDefines.h>

class CEntityManager
{
public:
	CEntity CreateNewEntity();
	CEntity CloneEntity(CEntity baseEntity);

private:
	entityx::EntityX m_entityX;
};

