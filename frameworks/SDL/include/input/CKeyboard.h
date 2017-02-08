#pragma once

#include <input/IKeyboard.h>

#include <array>

namespace Input
{
	class CKeyboard : public IKeyboard
	{
	public:
		CKeyboard();

		bool Init() override;
		void Destroy() override;

		void Update(float dt) override;

		bool IsPressed(EKeyboardKey key) override;
		bool BecomesPressed(EKeyboardKey key) override;
		bool IsReleased(EKeyboardKey key) override;
		bool BecomesReleased(EKeyboardKey key) override;

	private:
		const Uint8* mCurentKeyStates;
		const Uint8* mOldKeyStates;
	};
}
