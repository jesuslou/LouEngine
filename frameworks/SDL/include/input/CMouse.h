#pragma once

#include <input/IMouse.h>

#include <SDL_events.h>

#include <array>

namespace Input
{
	class CMouse : public IMouse
	{
	public:
		CMouse();

		bool Init() override;
		void Destroy() override;

		void Update(float dt) override;

		bool IsPressed(EMouseButton button) override;
		bool BecomesPressed(EMouseButton button) override;
		bool IsReleased(EMouseButton button) override;
		bool BecomesReleased(EMouseButton button) override;

		CVector2i GetMouseGlobalPosition() override;
		CVector2i GetMouseScreenPosition() override;

	private:
		bool IsMouseButtonPressed(Uint32 buttonState, EMouseButton button);

		bool m_currentMouseState[MB_N_MOUSE_BUTTONS];
		bool m_oldMouseState[MB_N_MOUSE_BUTTONS];

		CVector2i m_mousePosition;
	};
}
