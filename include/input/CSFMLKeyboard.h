#pragma once

#include <input/IKeyboard.h>

#include <SFML/Window/Keyboard.hpp>

#include <array>

namespace Input
{
	class CSFMLKeyboard : public IKeyboard
	{
	public:
		CSFMLKeyboard();

		bool Init() override;
		void Destroy() override;

		void Update(float dt) override;

		bool IsPressed(int key) override;
		bool BecomesPressed(int key) override;
		bool IsReleased(int key) override;
		bool BecomesReleased(int key) override;

	private:
		std::array<bool, sf::Keyboard::KeyCount> m_currentState;
		std::array<bool, sf::Keyboard::KeyCount> m_previousState;
	};
}
