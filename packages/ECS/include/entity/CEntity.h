////////////////////////////////////////////////////////////
//
// LouEngine - Multiplatform Game Engine Project
// Copyright (C) 2016-2017 Jesus Lou (loucocito@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

#include <handle/CHandle.h>
#include <common/CECSElement.h>
#include <hash/CStrID.h>
#include <tags/CTagsManager.h>

#include <vector>

class CComponent;
class CComponentFactoryManager;

class CEntity : public CECSElement
{
	template<class CEntity> friend class CFactory;
public:
	operator CHandle();
	const CEntity* operator=(const CHandle& rhs);

	template<typename T>
	CHandle AddComponent()
	{
		CComponent* component = m_componentFactoryManager.AddComponent<T>(m_components);
		if (component)
		{
			component->SetOwner(this);
		}
		return component;
	}
	template<typename T>
	bool RemoveComponent()
	{
		if (!m_components.empty())
		{
			int componentIdx = m_componentFactoryManager.GetFactoryindex<T>();
			if (m_components[componentIdx])
			{
				return m_componentFactoryManager.DestroyComponent(&m_components[componentIdx]);
			}
		}
		return false;
	}
	template<typename T>
	CHandle GetComponent()
	{
		int componentIdx = m_componentFactoryManager.GetFactoryindex<T>();
		if (componentIdx >= 0)
		{
			return m_components[componentIdx];
		}
		return CHandle();
	}

	void SetParent(CEntity* newParent);
	CHandle GetParent() const { return m_parent; }
	bool AddChild(CHandle newChild);
	bool RemoveChild(CHandle child);
	bool HasChild(CHandle child) const;
	int GetChildrenCount() const { return m_children.size(); }
	CHandle GetChildByName(const std::string& name);
	CHandle GetChildByIndex(std::size_t index);

	CHandle AddComponent(CStrID nameId);
	bool RemoveComponent(CStrID nameId);
	CHandle GetComponent(CStrID nameId);

	template<typename... Args>
	bool AddTags(Args... args)
	{
		return m_tagsManager.AddTags(m_tags, std::forward<Args>(args)...);
	}
	template<typename... Args>
	bool RemoveTags(Args... args)
	{
		return m_tagsManager.RemoveTags(m_tags, std::forward<Args>(args)...);
	}
	template<typename... Args>
	bool HasTags(Args... args)
	{
		return m_tagsManager.HasAllTags(m_tags, std::forward<Args>(args)...);
	}
	template<typename... Args>
	bool HasAnyTag(Args... args)
	{
		return m_tagsManager.HasAnyTags(m_tags, std::forward<Args>(args)...);
	}
	template<typename... Args>
	std::vector<CHandle> GetChildrenWithTags(Args... args)
	{
		std::vector<CHandle> children;
		for (CEntity* child : m_children)
		{
			if (child && child->HasTags(std::forward<Args>(args)...))
			{
				children.emplace_back(child);
			}
		}
		return children;
	}
	template<typename... Args>
	std::vector<CHandle> GetChildrenWithTagsRecursive(Args... args)
	{
		std::vector<CHandle> children;
		for (CEntity* child : m_children)
		{
			if (child)
			{
				if (child->HasTags(std::forward<Args>(args)...))
				{
					children.emplace_back(child);
				}
				child->GetChildrenWithTagsRecursiveInternal(children, std::forward<Args>(args)...);
			}
		}
		return children;
	}
	template<typename... Args>
	std::vector<CHandle> GetChildrenWithAnyTag(Args... args)
	{
		std::vector<CHandle> children;
		for (CEntity* child : m_children)
		{
			if (child && child->HasAnyTag(std::forward<Args>(args)...))
			{
				children.emplace_back(child);
			}
		}
		return children;
	}
	template<typename... Args>
	std::vector<CHandle> GetChildrenWithAnyTagRecursive(Args... args)
	{
		std::vector<CHandle> children;
		for (CEntity* child : m_children)
		{
			if (child)
			{
				if (child->HasAnyTag(std::forward<Args>(args)...))
				{
					children.emplace_back(child);
				}
				child->GetChildrenWithAnyTagRecursiveInternal(children, std::forward<Args>(args)...);
			}
		}
		return children;
	}

	void Init() override;
	void Destroy() override;

	void Activate() override;
	void Deactivate() override;

	bool IsInitialized() const { return m_initialized; }
	bool IsActive() const { return m_numDeactivations == 0; }
	bool IsDestroyed() const { return m_destroyed; }

	void SetName(const char* const name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

private:
	CEntity();
	~CEntity();

	void ActivateFromParent();
	void DeactivateFromParent();

	template<typename... Args>
	void GetChildrenWithTagsRecursiveInternal(std::vector<CHandle>& children, Args... args)
	{
		for (CEntity* child : m_children)
		{
			if (child)
			{
				if (child->HasTags(std::forward<Args>(args)...))
				{
					children.emplace_back(child);
				}
				child->GetChildrenWithTagsRecursiveInternal(children, std::forward<Args>(args)...);
			}
		}
	}

	template<typename... Args>
	void GetChildrenWithAnyTagRecursiveInternal(std::vector<CHandle>& children, Args... args)
	{
		for (CEntity* child : m_children)
		{
			if (child)
			{
				if (child->HasAnyTag(std::forward<Args>(args)...))
				{
					children.emplace_back(child);
				}
				child->GetChildrenWithAnyTagRecursiveInternal(children, std::forward<Args>(args)...);
			}
		}
	}

	CHandle m_parent;
	std::vector<CHandle> m_children;

	std::vector<CComponent*> m_components;

	CComponentFactoryManager& m_componentFactoryManager;
	CTagsManager& m_tagsManager;

	TagsMask m_tags;

	std::string m_name;

	int m_numDeactivations;
	bool m_initialized;
	bool m_destroyed;
};
