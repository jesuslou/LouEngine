#pragma once

#include <glm/glm.hpp>
#include <SDL_scancode.h>

#define CVector2i glm::ivec2
#define CVector2f glm::vec2

enum EKeys
{
	VK_SPACE = SDL_SCANCODE_SPACE
	, VK_ENTER = SDL_SCANCODE_RETURN
	, VK_UP = SDL_SCANCODE_UP
	, VK_DOWN = SDL_SCANCODE_DOWN
	, VK_LEFT = SDL_SCANCODE_LEFT
	, VK_RIGHT = SDL_SCANCODE_RIGHT
	, VK_W = SDL_SCANCODE_W
	, VK_A = SDL_SCANCODE_A
	, VK_S = SDL_SCANCODE_S
	, VK_D = SDL_SCANCODE_D
	, VK_0 = SDL_SCANCODE_0
	, VK_1 = SDL_SCANCODE_1
	, VK_2 = SDL_SCANCODE_2
	, VK_3 = SDL_SCANCODE_3
	, VK_4 = SDL_SCANCODE_4
	, VK_5 = SDL_SCANCODE_5
};

#define LKey EKeys
