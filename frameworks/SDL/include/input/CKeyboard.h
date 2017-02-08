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

		bool IsPressed(LKey key) override;
		bool BecomesPressed(LKey key) override;
		bool IsReleased(LKey key) override;
		bool BecomesReleased(LKey key) override;

	private:
		const Uint8* mCurentKeyStates;
		const Uint8* mOldKeyStates;
	};
}
