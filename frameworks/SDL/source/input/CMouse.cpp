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
		for (size_t i = 0; i < MB_N_MOUSE_BUTTONS; ++i)
		{
			m_oldMouseState[i] = m_currentMouseState[i];
		}
	}

	void CMouse::updateSDLMouse(SDL_Event & event)
	{
		if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
		{
			m_currentMouseState[event.button.button] = (event.type == SDL_MOUSEBUTTONDOWN);
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			m_mousePosition = CVector2i(event.motion.x, event.motion.y);
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
		CVector2i mousePosition;
		SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
		return mousePosition;
	}

}
