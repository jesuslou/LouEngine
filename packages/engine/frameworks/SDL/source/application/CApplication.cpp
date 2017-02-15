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

#include <LouEnginePrecompile.h>

#include <application/CApplication.h>
#include <application/SApplicationWindowParameters.h>
#include <input/CKeyboard.h>
#include <input/CMouse.h>
#include <graphics/CRenderer.h>
#include <systems/CSystems.h>

#include <SDL.h>
#include <SDL_mixer.h>

CApplication::CApplication()
	: m_window(nullptr)
{
}

CApplication::~CApplication()
{
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	SDL_Quit();
}

bool CApplication::Init(const SApplicationWindowParameters& applicationWindowParameters)
{
	m_keyboard = new Input::CKeyboard();
	if (!m_keyboard->Init())
	{
		DELETE_POINTER(m_keyboard);
		return false;
	}
	CSystems::SetSystem<Input::IKeyboard>(m_keyboard);

	m_mouse = new Input::CMouse();
	if (!m_mouse->Init())
	{
		DELETE_POINTER(m_mouse);
		return false;
	}
	CSystems::SetSystem<Input::IMouse>(m_mouse);

	if (InitSDL(applicationWindowParameters))
	{
		return InitProject(m_gameSystems);
	}
	return false;
}

void CApplication::Update() 
{
	SDL_Event event;
	Uint64 timeNow = SDL_GetPerformanceCounter();
	Uint64 timeLast = timeNow;
	float deltaTime = 0.f;

	while (true)
	{
		Uint64 diff = timeNow - timeLast;
		deltaTime = static_cast<float>(timeNow - timeLast) * 1000.f / static_cast<float>(SDL_GetPerformanceFrequency()) / 1000.f;
		timeLast = timeNow;

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				return;
			}
		}
		m_mouse->Update(deltaTime);
		m_keyboard->Update(deltaTime);

		UpdateProject(deltaTime);

		timeNow = SDL_GetPerformanceCounter();
	}
}

void CApplication::Destroy() 
{
	DestroyProject();
	m_renderer->Destroy();
	CSystems::DestroySystem<IRenderer>();
	m_keyboard->Destroy();
	CSystems::DestroySystem<Input::IKeyboard>();
	m_mouse->Destroy();
	CSystems::DestroySystem<Input::IMouse>();

	SDL_Quit();
}

bool CApplication::InitSDL(const SApplicationWindowParameters& applicationWindowParameters)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("CAudioManager::SDL Mixer initialization FAILURE! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("CRenderer::SDL initialization FAILURE! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		printf("CRenderer::SDL initialization SUCCESS!\n");
		//Create window
		m_window = SDL_CreateWindow(applicationWindowParameters.m_windowTitle.c_str()
									, SDL_WINDOWPOS_UNDEFINED
									, SDL_WINDOWPOS_UNDEFINED, applicationWindowParameters.m_xRes
									, applicationWindowParameters.m_yRes
									, SDL_WINDOW_SHOWN);
		if (m_window == nullptr)
		{
			printf("CRenderer::Window creation FAILURE! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			m_renderer = new CRenderer(m_window);
			if (!m_renderer->Init(applicationWindowParameters))
			{
				DELETE_POINTER(m_renderer);
				return false;
			}
			CSystems::SetSystem<IRenderer>(m_renderer);
		}
	}
	return true;
}
