#pragma once

#include <SFML/Window/Mouse.hpp>

namespace Input
{
	enum EMouseButton
	{
		MB_INVALID = -1,
		MB_LEFT = sf::Mouse::Left,
		MB_MIDDLE = sf::Mouse::Middle,
		MB_RIGHT = sf::Mouse::Right,
		MB_X1 = sf::Mouse::XButton1,
		MB_X2 = sf::Mouse::XButton2,

		MB_N_MOUSE_BUTTONS = sf::Mouse::ButtonCount
	};
}
