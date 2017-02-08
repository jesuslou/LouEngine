#pragma once

#include <input/EMouseButton.h>

namespace Input
{
	class IMouse
	{
	public:
		IMouse() {}
		virtual ~IMouse() {}

		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void Update(float dt) = 0;

		virtual bool IsPressed(EMouseButton button) = 0;
		virtual bool BecomesPressed(EMouseButton button) = 0;
		virtual bool IsReleased(EMouseButton button) = 0;
		virtual bool BecomesReleased(EMouseButton button) = 0;

		virtual CVector2i GetMouseGlobalPosition() = 0;
		virtual CVector2i GetMouseScreenPosition() = 0;
	};
}
