#include <LouEnginePrecompile.h>

#include <input/CMouse.h>

#include <SDL_mouse.h>

namespace Input
{
	CMouse::CMouse()
	{
	}

	bool CMouse::Init()
	{
		return true;
	}

	void CMouse::Destroy()
	{
	}

	void CMouse::Update(float /*dt*/)
	{
		Uint32 currentMouseState = SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
		for (int i = 0; i < MB_N_MOUSE_BUTTONS; ++i)
		{
			m_oldMouseState[i] = m_currentMouseState[i];
			m_currentMouseState[i] = IsMouseButtonPressed(currentMouseState, static_cast<EMouseButton>(i));
		}
	}

	bool CMouse::IsPressed(EMouseButton button)
	{
		return m_currentMouseState[button];
	}

	bool CMouse::BecomesPressed(EMouseButton button)
	{
		return !m_oldMouseState[button] && m_currentMouseState[button];
	}

	bool CMouse::IsReleased(EMouseButton button)
	{
		return !m_currentMouseState[button];
	}

	bool CMouse::BecomesReleased(EMouseButton button)
	{
		return m_oldMouseState[button] && !!m_currentMouseState[button];
	}

	CVector2i CMouse::GetMouseGlobalPosition()
	{
		CVector2i mousePosition;
		SDL_GetGlobalMouseState(&mousePosition.x, &mousePosition.y);
		return mousePosition;
	}

	CVector2i CMouse::GetMouseScreenPosition()
	{
		return m_mousePosition;
	}

	bool CMouse::IsMouseButtonPressed(Uint32 buttonState, EMouseButton button)
	{
		return (buttonState & SDL_BUTTON(button) ? true : false);
	}
}
