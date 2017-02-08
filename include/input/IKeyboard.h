#pragma once

#include <input/EKeyboardKey.h>

namespace Input
{
	class IKeyboard
	{
	public:
		IKeyboard() {}
		virtual ~IKeyboard() {}

		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void Update(float dt) = 0;

		virtual bool IsPressed(EKeyboardKey key) = 0;
		virtual bool BecomesPressed(EKeyboardKey key) = 0;
		virtual bool IsReleased(EKeyboardKey key) = 0;
		virtual bool BecomesReleased(EKeyboardKey key) = 0;
	};
}
