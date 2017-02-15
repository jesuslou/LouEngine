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

#include <SFML/Window/Keyboard.hpp>

namespace Input
{
	enum EKeyboardKey
	{
		KK_INVALID = sf::Keyboard::Unknown,

		KK_A = sf::Keyboard::A,
		KK_B = sf::Keyboard::B,
		KK_C = sf::Keyboard::C,
		KK_D = sf::Keyboard::D,
		KK_E = sf::Keyboard::E,
		KK_F = sf::Keyboard::F,
		KK_G = sf::Keyboard::G,
		KK_H = sf::Keyboard::H,
		KK_I = sf::Keyboard::I,
		KK_J = sf::Keyboard::J,
		KK_K = sf::Keyboard::K,
		KK_L = sf::Keyboard::L,
		KK_M = sf::Keyboard::M,
		KK_N = sf::Keyboard::N,
		KK_O = sf::Keyboard::O,
		KK_P = sf::Keyboard::P,
		KK_Q = sf::Keyboard::Q,
		KK_R = sf::Keyboard::R,
		KK_S = sf::Keyboard::S,
		KK_T = sf::Keyboard::T,
		KK_U = sf::Keyboard::U,
		KK_V = sf::Keyboard::V,
		KK_W = sf::Keyboard::W,
		KK_X = sf::Keyboard::X,
		KK_Y = sf::Keyboard::Y,
		KK_Z = sf::Keyboard::Z,

		KK_1 = sf::Keyboard::Num1,
		KK_2 = sf::Keyboard::Num2,
		KK_3 = sf::Keyboard::Num3,
		KK_4 = sf::Keyboard::Num4,
		KK_5 = sf::Keyboard::Num5,
		KK_6 = sf::Keyboard::Num6,
		KK_7 = sf::Keyboard::Num7,
		KK_8 = sf::Keyboard::Num8,
		KK_9 = sf::Keyboard::Num9,
		KK_0 = sf::Keyboard::Num0,

		KK_RETURN = sf::Keyboard::Return,
		KK_SCAPE = sf::Keyboard::Escape,
		KK_BACKSPACE = sf::Keyboard::BackSpace,
		KK_TAB = sf::Keyboard::Tab,
		KK_SPACE = sf::Keyboard::Space,

		KK_F1 = sf::Keyboard::F1,
		KK_F2 = sf::Keyboard::F2,
		KK_F3 = sf::Keyboard::F3,
		KK_F4 = sf::Keyboard::F4,
		KK_F5 = sf::Keyboard::F5,
		KK_F6 = sf::Keyboard::F6,
		KK_F7 = sf::Keyboard::F7,
		KK_F8 = sf::Keyboard::F8,
		KK_F9 = sf::Keyboard::F9,
		KK_F10 = sf::Keyboard::F10,
		KK_F11 = sf::Keyboard::F11,
		KK_F12 = sf::Keyboard::F12,

		KK_RIGHT = sf::Keyboard::Right,
		KK_LEFT = sf::Keyboard::Left,
		KK_DOWN = sf::Keyboard::Down,
		KK_UP = sf::Keyboard::Up
	};
}
