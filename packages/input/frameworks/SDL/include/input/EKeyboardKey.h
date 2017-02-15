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

#include <SDL_scancode.h>

namespace Input
{
	enum EKeyboardKey
	{
		KK_INVALID = SDL_SCANCODE_UNKNOWN,

		KK_A = SDL_SCANCODE_A,
		KK_B = SDL_SCANCODE_B,
		KK_C = SDL_SCANCODE_C,
		KK_D = SDL_SCANCODE_D,
		KK_E = SDL_SCANCODE_E,
		KK_F = SDL_SCANCODE_F,
		KK_G = SDL_SCANCODE_G,
		KK_H = SDL_SCANCODE_H,
		KK_I = SDL_SCANCODE_I,
		KK_J = SDL_SCANCODE_J,
		KK_K = SDL_SCANCODE_K,
		KK_L = SDL_SCANCODE_L,
		KK_M = SDL_SCANCODE_M,
		KK_N = SDL_SCANCODE_N,
		KK_O = SDL_SCANCODE_O,
		KK_P = SDL_SCANCODE_P,
		KK_Q = SDL_SCANCODE_Q,
		KK_R = SDL_SCANCODE_R,
		KK_S = SDL_SCANCODE_S,
		KK_T = SDL_SCANCODE_T,
		KK_U = SDL_SCANCODE_U,
		KK_V = SDL_SCANCODE_V,
		KK_W = SDL_SCANCODE_W,
		KK_X = SDL_SCANCODE_X,
		KK_Y = SDL_SCANCODE_Y,
		KK_Z = SDL_SCANCODE_Z,

		KK_1 = SDL_SCANCODE_1,
		KK_2 = SDL_SCANCODE_2,
		KK_3 = SDL_SCANCODE_3,
		KK_4 = SDL_SCANCODE_4,
		KK_5 = SDL_SCANCODE_5,
		KK_6 = SDL_SCANCODE_6,
		KK_7 = SDL_SCANCODE_7,
		KK_8 = SDL_SCANCODE_8,
		KK_9 = SDL_SCANCODE_9,
		KK_0 = SDL_SCANCODE_0,

		KK_RETURN = SDL_SCANCODE_RETURN,
		KK_SCAPE = SDL_SCANCODE_ESCAPE,
		KK_BACKSPACE = SDL_SCANCODE_BACKSPACE,
		KK_TAB = SDL_SCANCODE_TAB,
		KK_SPACE = SDL_SCANCODE_SPACE,

		KK_F1 = SDL_SCANCODE_F1,
		KK_F2 = SDL_SCANCODE_F2,
		KK_F3 = SDL_SCANCODE_F3,
		KK_F4 = SDL_SCANCODE_F4,
		KK_F5 = SDL_SCANCODE_F5,
		KK_F6 = SDL_SCANCODE_F6,
		KK_F7 = SDL_SCANCODE_F7,
		KK_F8 = SDL_SCANCODE_F8,
		KK_F9 = SDL_SCANCODE_F9,
		KK_F10 = SDL_SCANCODE_F10,
		KK_F11 = SDL_SCANCODE_F11,
		KK_F12 = SDL_SCANCODE_F12,

		KK_RIGHT = SDL_SCANCODE_RIGHT,
		KK_LEFT = SDL_SCANCODE_LEFT,
		KK_DOWN = SDL_SCANCODE_DOWN,
		KK_UP = SDL_SCANCODE_UP
	};
}
