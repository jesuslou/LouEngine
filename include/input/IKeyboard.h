#pragma once

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

		virtual bool IsPressed(LKey key) = 0;
		virtual bool BecomesPressed(LKey key) = 0;
		virtual bool IsReleased(LKey key) = 0;
		virtual bool BecomesReleased(LKey key) = 0;
	};
}
