#pragma once

#include <entities/EntityDefines.h>

class IComponentFactory
{
public:
	virtual CEntity AddComponent(CEntity entity) = 0;
	virtual CEntity AddComponent(CEntity entity, char* json) = 0;
};

template<typename T>
class CComponentFactory : public IComponentFactory
{
public:
	CEntity AddComponent(CEntity entity) override
	{
		CComponentHandle<T> component = entity.assign<T>();
		return entity;
	}

	CEntity AddComponent(CEntity entity, char* json) override
	{
		CComponentHandle<T> component = entity.assign<T>();
		T* componentPtr = component.get();
		componentPtr->ParseAtts(/*json*/);
		return entity;
	}
};
