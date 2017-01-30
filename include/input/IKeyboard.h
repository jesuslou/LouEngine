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

		virtual bool IsPressed(int key) = 0;
		virtual bool BecomesPressed(int key) = 0;
		virtual bool IsReleased(int key) = 0;
		virtual bool BecomesReleased(int key) = 0;
	};
}
