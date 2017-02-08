#include <LouEnginePrecompile.h>

#include <input/CKeyboard.h>

#include <SDL_keyboard.h>

namespace Input
{
	CKeyboard::CKeyboard()
		: mCurentKeyStates(nullptr)
		, mOldKeyStates(nullptr)
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
		int n = 0;
		if (!mOldKeyStates)
		{
			const Uint8* tmpKeyStates = SDL_GetKeyboardState(&n);
			mCurentKeyStates = new Uint8[n];
			mOldKeyStates = new Uint8[n];
			memcpy((void*)mCurentKeyStates, tmpKeyStates, sizeof(Uint8) * n);
			memset((void*)mOldKeyStates, 0x0, sizeof(Uint8) * n);
		}
		else
		{
			const Uint8* tmpKeyStates = SDL_GetKeyboardState(&n);
			memcpy((void*)mOldKeyStates, mCurentKeyStates, sizeof(Uint8) * n);
			memcpy((void*)mCurentKeyStates, tmpKeyStates, sizeof(Uint8) * n);
		}
	}

	bool CKeyboard::IsPressed(EKeyboardKey key)
	{
		return mCurentKeyStates[key] != 0;
	}

	bool CKeyboard::BecomesPressed(EKeyboardKey key)
	{
		return mOldKeyStates[key] == 0 && mCurentKeyStates[key] != 0;
	}

	bool CKeyboard::IsReleased(EKeyboardKey key)
	{
		return mCurentKeyStates[key] == 0;
	}

	bool CKeyboard::BecomesReleased(EKeyboardKey key)
	{
		return mOldKeyStates[key] != 0 && !mCurentKeyStates[key] == 0;
	}
}
