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

namespace Json
{
	class Value;
}

class CComponent : public CECSElement
{
	template<class CComponent> friend class CFactory;
public:
	virtual ~CComponent() {}

	void Init() override;
	void Update(float dt) override;
	void Destroy() override;

	operator CHandle();
	const CComponent* operator=(const CHandle& rhs);

	void SetOwner(CHandle owner) { m_owner = owner; }
	CHandle GetOwner() const { return m_owner; }

	void Activate() override;
	void Deactivate() override;

	bool IsInitialized() const { return m_initialized; }
	bool IsActive() const { return m_numDeactivations == 0; }
	bool IsDestroyed() const { return m_destroyed; }

	virtual void ParseAtts(const Json::Value& atts) {}

	bool GetIsInitiallyActive() const { return m_initiallyActive; }
	void SetIsInitiallyActive(bool initiallyActive) { m_initiallyActive = initiallyActive; }
	void CheckFirstActivation();
	void ActivateFromParent();

protected:
	CComponent();

	void CheckFirstActivationInternal();

	virtual void DoInit() {}
	virtual void DoUpdate(float dt) {}
	virtual void DoDestroy() {}
	virtual void DoActivate() {}
	virtual void DoDeactivate() {}

	CHandle m_owner;

	int m_numDeactivations;
	bool m_initialized;
	bool m_destroyed;
	bool m_initiallyActive;
};
