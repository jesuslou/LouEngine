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

#include <SDL_keyboard.h>

namespace Input
{
	CKeyboard::CKeyboard()
		: m_curentKeyStates(nullptr)
		, m_oldKeyStates(nullptr)
	{
	}

	bool CKeyboard::Init()
	{
		return true;
	}

	void CKeyboard::Destroy()
	{
		if (m_oldKeyStates)
		{
			delete[] m_oldKeyStates;
			m_oldKeyStates = nullptr;
		}

		if (m_curentKeyStates)
		{
			delete[] m_curentKeyStates;
			m_curentKeyStates = nullptr;
		}
	}

	void CKeyboard::Update(float /*dt*/)
	{
		int n = 0;
		if (!m_oldKeyStates)
		{
			const Uint8* tmpKeyStates = SDL_GetKeyboardState(&n);
			m_curentKeyStates = new Uint8[n];
			m_oldKeyStates = new Uint8[n];
			memcpy((void*)m_curentKeyStates, tmpKeyStates, sizeof(Uint8) * n);
			memset((void*)m_oldKeyStates, 0x0, sizeof(Uint8) * n);
		}
		else
		{
			const Uint8* tmpKeyStates = SDL_GetKeyboardState(&n);
			memcpy((void*)m_oldKeyStates, m_curentKeyStates, sizeof(Uint8) * n);
			memcpy((void*)m_curentKeyStates, tmpKeyStates, sizeof(Uint8) * n);
		}
	}

	bool CKeyboard::IsPressed(EKeyboardKey key)
	{
		return m_curentKeyStates[key] != 0;
	}

	bool CKeyboard::BecomesPressed(EKeyboardKey key)
	{
		return m_oldKeyStates[key] == 0 && m_curentKeyStates[key] != 0;
	}

	bool CKeyboard::IsReleased(EKeyboardKey key)
	{
		return m_curentKeyStates[key] == 0;
	}

	bool CKeyboard::BecomesReleased(EKeyboardKey key)
	{
		return m_oldKeyStates[key] != 0 && !m_curentKeyStates[key] == 0;
	}
}
