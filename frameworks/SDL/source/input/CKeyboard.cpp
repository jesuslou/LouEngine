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
