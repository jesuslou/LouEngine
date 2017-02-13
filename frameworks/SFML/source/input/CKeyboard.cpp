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

#include <input/CKeyboard.h>

namespace Input
{
	CKeyboard::CKeyboard()
		: m_currentState()
		, m_previousState()
	{
	}

	bool CKeyboard::Init()
	{
		return true;
	}

	void CKeyboard::Destroy()
	{
	}

	void CKeyboard::Update(float /*dt*/)
	{
		for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
		{
			m_previousState[i] = m_currentState[i];
			m_currentState[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
		}
	}

	bool CKeyboard::IsPressed(EKeyboardKey key)
	{
		return m_currentState[key];
	}

	bool CKeyboard::BecomesPressed(EKeyboardKey key)
	{
		return m_currentState[key] && !m_previousState[key];
	}

	bool CKeyboard::IsReleased(EKeyboardKey key)
	{
		return !m_currentState[key];
	}

	bool CKeyboard::BecomesReleased(EKeyboardKey key)
	{
		return !m_currentState[key] && m_previousState[key];
	}
}
