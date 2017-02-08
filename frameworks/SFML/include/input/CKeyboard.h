#pragma once

#include <input/IKeyboard.h>

#include <SFML/Window/Keyboard.hpp>

#include <array>

namespace Input
{
	class CKeyboard : public IKeyboard
	{
	public:
		CKeyboard();

		bool Init() override;
		void Destroy() override;

		void Update(float dt) override;

		bool IsPressed(EKeyboardKey key) override;
		bool BecomesPressed(EKeyboardKey key) override;
		bool IsReleased(EKeyboardKey key) override;
		bool BecomesReleased(EKeyboardKey key) override;

	private:
		std::array<bool, sf::Keyboard::KeyCount> m_currentState;
		std::array<bool, sf::Keyboard::KeyCount> m_previousState;
	};
}
