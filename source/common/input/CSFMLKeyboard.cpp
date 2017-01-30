#include <LouEnginePrecompile.h>

#include <input/CSFMLKeyboard.h>

namespace Input
{
	CSFMLKeyboard::CSFMLKeyboard()
		: m_currentState()
		, m_previousState()
	{
	}

	bool CSFMLKeyboard::Init()
	{
		return true;
	}

	void CSFMLKeyboard::Destroy()
	{
	}

	void CSFMLKeyboard::Update(float /*dt*/)
	{
		for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
		{
			m_previousState[i] = m_currentState[i];
			m_currentState[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
		}
	}

	bool CSFMLKeyboard::IsPressed(int key)
	{
		return m_currentState[key];
	}

	bool CSFMLKeyboard::BecomesPressed(int key)
	{
		return m_currentState[key] && !m_previousState[key];
	}

	bool CSFMLKeyboard::IsReleased(int key)
	{
		return !m_currentState[key];
	}

	bool CSFMLKeyboard::BecomesReleased(int key)
	{
		return !m_currentState[key] && m_previousState[key];
	}
}
