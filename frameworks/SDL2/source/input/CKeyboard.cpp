#include <LouEnginePrecompile.h>

#include <input/CKeyboard.h>

namespace Input
{
	CKeyboard::CKeyboard()
		: m_currentState()
		, m_previousState()
	{
	}

	bool CKeyboard::Init()
	{
		return true;
	}

	void CKeyboard::Destroy()
	{
	}

	void CKeyboard::Update(float /*dt*/)
	{
		for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
		{
			m_previousState[i] = m_currentState[i];
			m_currentState[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
		}
	}

	bool CKeyboard::IsPressed(LKey key)
	{
		return m_currentState[key];
	}

	bool CKeyboard::BecomesPressed(LKey key)
	{
		return m_currentState[key] && !m_previousState[key];
	}

	bool CKeyboard::IsReleased(LKey key)
	{
		return !m_currentState[key];
	}

	bool CKeyboard::BecomesReleased(LKey key)
	{
		return !m_currentState[key] && m_previousState[key];
	}
}