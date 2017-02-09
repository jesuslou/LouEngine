#include <LouEnginePrecompile.h>

#include <input/CMouse.h>

#include <SFML/Window/Mouse.hpp>

namespace Input
{
	CMouse::CMouse()
		: m_window(nullptr)
	{
	}

	CMouse::CMouse(sf::Window* window)
		: m_window(window)
	{}

	bool CMouse::Init()
	{
		return true;
	}

	void CMouse::Destroy()
	{
	}

	void CMouse::Update(float /*dt*/)
	{
		for (int i = 0; i < MB_N_MOUSE_BUTTONS; ++i)
		{
			m_oldMouseState[i] = m_currentMouseState[i];
			m_currentMouseState[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
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
		return m_oldMouseState[button] && !m_currentMouseState[button];
	}

	CVector2i CMouse::GetMouseGlobalPosition()
	{
		return sf::Mouse::getPosition();
	}

	CVector2i CMouse::GetMouseScreenPosition()
	{
		return m_window ? sf::Mouse::getPosition(*m_window) : CVector2i(-1, -1);
	}
}
