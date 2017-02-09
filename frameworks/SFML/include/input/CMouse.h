#pragma once

#include <input/IMouse.h>

namespace sf
{
	class Window;
}

namespace Input
{
	class CMouse : public IMouse
	{
	public:
		CMouse();
		CMouse(sf::Window* window);

		bool Init() override;
		void Destroy() override;

		void Update(float dt) override;

		bool IsPressed(EMouseButton button) override;
		bool BecomesPressed(EMouseButton button) override;
		bool IsReleased(EMouseButton button) override;
		bool BecomesReleased(EMouseButton button) override;

		CVector2i GetMouseGlobalPosition() override;
		CVector2i GetMouseScreenPosition() override;

		void SetWindow(sf::Window* window) { m_window = window; }

	private:
		bool m_currentMouseState[MB_N_MOUSE_BUTTONS];
		bool m_oldMouseState[MB_N_MOUSE_BUTTONS];

		sf::Window* m_window;
	};
}
