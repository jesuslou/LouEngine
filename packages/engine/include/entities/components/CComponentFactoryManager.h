#pragma once

#include <entities/EntityDefines.h>
#include <entities/components/CComponentFactory.h>
#include <entities/components/IComponent.h>

#include <type_traits>

#define REGISTER_COMPONENT_FACTORY(name, T) CSystems::GetSystem<CComponentFactoryManager>()->RegisterFactory(std::string(name), new CComponentFactory<T>());

class CComponentFactoryManager
{
public:
	~CComponentFactoryManager()
	{
		for (auto& factory : m_factories)
		{
			delete factory.second;
		}
		m_factories.clear();
	}

	std::unordered_map<std::string, IComponentFactory*> m_factories;

	template<typename T>
	typename std::enable_if<std::is_base_of<IComponent, T>::value>::type
	RegisterFactory(const std::string& name, CComponentFactory<T>* factory)
	{
		if (m_factories.find(name) == m_factories.end())
		{
			m_factories[name] = factory;
		}
	}

	CEntity AddComponentByStr(const std::string& compName, CEntity entity)
	{
		if (m_factories.find(compName) != m_factories.end())
		{
			m_factories[compName]->AddComponent(entity);
		}
		return entity;
	}
};
