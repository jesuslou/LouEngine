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

#include <application/SApplicationWindowParameters.h>
#include <graphics/CRenderer.h>

#include <SDL_render.h>

CRenderer::CRenderer()
	: m_renderer(nullptr)
	, m_clearColor()
{
	m_clearColor.r = 0;
	m_clearColor.g = 0;
	m_clearColor.b = 0;
	m_clearColor.a = 1;
}

CRenderer::CRenderer(SDL_Window* window)
	: m_window(window)
	, m_renderer(nullptr)
	, m_clearColor()
{
	m_clearColor.r = 0;
	m_clearColor.g = 0;
	m_clearColor.b = 0;
	m_clearColor.a = 1;
}

CRenderer::~CRenderer()
{
	Destroy();
}

bool CRenderer::Init(const SApplicationWindowParameters& applicationWindowParameters)
{
	Uint32 flags = SDL_RENDERER_ACCELERATED;
	flags |= applicationWindowParameters.m_hasVerticalSync ? SDL_RENDERER_PRESENTVSYNC : 0;
	m_renderer = SDL_CreateRenderer(m_window, -1, flags);
	if (m_renderer)
	{
		SDL_SetRenderDrawColor(m_renderer, m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		return true;//CTextureManager::get().init();
	}
	return false;
}

void CRenderer::Destroy()
{
	if (m_renderer)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void CRenderer::Render()
{

}
