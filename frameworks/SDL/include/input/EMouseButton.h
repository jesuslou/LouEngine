#pragma once

#include <SDL_mouse.h>

namespace Input
{
	enum EMouseButton
	{
		MB_INVALID = 0,
		MB_LEFT = SDL_BUTTON_LEFT,
		MB_MIDDLE = SDL_BUTTON_MIDDLE,
		MB_RIGHT = SDL_BUTTON_RIGHT,
		MB_X1 = SDL_BUTTON_X1,
		MB_X2 = SDL_BUTTON_X2,

		MB_N_MOUSE_BUTTONS = 5
	};
}
