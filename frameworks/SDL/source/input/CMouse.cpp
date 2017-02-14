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

#include <LouEnginePrecompile.h>

#include <input/CMouse.h>

#include <SDL_mouse.h>

namespace Input
{
	CMouse::CMouse()
	{
	}

	bool CMouse::Init()
	{
		return true;
	}

	void CMouse::Destroy()
	{
	}

	void CMouse::Update(float /*dt*/)
	{
		Uint32 currentMouseState = SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
		for (int i = 0; i < MB_N_MOUSE_BUTTONS; ++i)
		{
			m_oldMouseState[i] = m_currentMouseState[i];
			m_currentMouseState[i] = IsMouseButtonPressed(currentMouseState, static_cast<EMouseButton>(i));
		}
	}

	bool CMouse::IsPressed(EMouseButton button)
	{
		return m_currentMouseState[button];
	}

	bool CMouse::BecomesPressed(EMouseButton button)
	{
		return !m_oldMouseState[button] && m_currentMouseState[button];
	}

	bool CMouse::IsReleased(EMouseButton button)
	{
		return !m_currentMouseState[button];
	}

	bool CMouse::BecomesReleased(EMouseButton button)
	{
		return m_oldMouseState[button] && !!m_currentMouseState[button];
	}

	CVector2i CMouse::GetMouseScreenPosition()
	{
		return m_mousePosition;
	}

	bool CMouse::IsMouseButtonPressed(Uint32 buttonState, EMouseButton button)
	{
		return (buttonState & SDL_BUTTON(button) ? true : false);
	}
}
