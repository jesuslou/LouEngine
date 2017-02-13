////////////////////////////////////////////////////////////
//
// LouEngine - Multiplatform Game Engine Project
// Copyright (C) 2016-2017 Jesus Lou (loucocito@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

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
