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

#include <systems/CSystems.h>

struct SApplicationWindowParameters;
class IRenderer;

namespace Input
{
	class IKeyboard;
	class IMouse;
}

class IApplication
{
public:
	IApplication() 
		: m_renderer(nullptr)
		, m_keyboard(nullptr)
		, m_mouse(nullptr)
	{
		CSystems::SetGameSystems(&m_gameSystems);
	}
	virtual ~IApplication() {}

	virtual bool Init(const SApplicationWindowParameters& applicationWindowParameters) = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

protected:
	virtual bool InitProject(CGameSystems& gameSystems) = 0;
	virtual void UpdateProject(float dt) = 0;
	virtual void DestroyProject() = 0;

	CGameSystems m_gameSystems;

	IRenderer* m_renderer;
	Input::IKeyboard* m_keyboard;
	Input::IMouse* m_mouse;
};
