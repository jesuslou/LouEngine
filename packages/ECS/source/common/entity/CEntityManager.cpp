#include <entity/CEntityManager.h>
#include <handle/CHandle.h>

CEntityManager::CEntityManager()
	: CFactory(CHandle::MAX_ELEMENTS)
{
}
