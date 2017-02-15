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

#include <SFML/Window/Mouse.hpp>

namespace Input
{
	CMouse::CMouse()
		: m_window(nullptr)
	{
	}

	CMouse::CMouse(sf::Window* window)
		: m_window(window)
	{}

	bool CMouse::Init()
	{
		return true;
	}

	void CMouse::Destroy()
	{
	}

	void CMouse::Update(float /*dt*/)
	{
		for (int i = 0; i < MB_N_MOUSE_BUTTONS; ++i)
		{
			m_oldMouseState[i] = m_currentMouseState[i];
			m_currentMouseState[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
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
		return m_oldMouseState[button] && !m_currentMouseState[button];
	}

	CVector2i CMouse::GetMouseScreenPosition()
	{
		return m_window ? sf::Mouse::getPosition(*m_window) : CVector2i(-1, -1);
	}
}
