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

#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

CApplication::CApplication()
{
}

CApplication::~CApplication()
{
	if (m_mainWindow)
	{
		delete m_mainWindow;
		m_mainWindow = nullptr;
	}
}

bool CApplication::Init(const SApplicationWindowParameters& applicationWindowParameters)
{
	m_renderer = new CRenderer();
	if (!m_renderer->Init())
	{
		delete m_renderer;
		return false;
	}
	CSystems::SetSystem<IRenderer>(m_renderer);

	m_keyboard = new Input::CKeyboard();
	if (!m_keyboard->Init())
	{
		delete m_keyboard;
		return false;
	}
	CSystems::SetSystem<Input::IKeyboard>(m_keyboard);

	sf::Uint32 windowStyle = applicationWindowParameters.m_hasTitleBar ? sf::Style::Titlebar : sf::Style::None;
	windowStyle |= applicationWindowParameters.m_hasCloseButton ? sf::Style::Close : sf::Style::None;
	windowStyle |= applicationWindowParameters.m_hasResizeButton ? sf::Style::Resize : sf::Style::None;
	windowStyle |= applicationWindowParameters.m_isFullScreen ? sf::Style::Fullscreen : sf::Style::None;

	m_mainWindow = new sf::RenderWindow(sf::VideoMode(applicationWindowParameters.m_xRes, applicationWindowParameters.m_yRes)
								  , applicationWindowParameters.m_windowTitle
								  , windowStyle
								  );

	m_mouse = new Input::CMouse(m_mainWindow);
	if (!m_mouse->Init())
	{
		delete m_mouse;
		return false;
	}
	CSystems::SetSystem<Input::IMouse>(m_mouse);

	if (applicationWindowParameters.m_hasVerticalSync)
	{
		m_mainWindow->setVerticalSyncEnabled(true);
	}
	else
	{
		m_mainWindow->setFramerateLimit(applicationWindowParameters.m_frameRateLimit);
	}

	return InitProject(m_gameSystems);
}

void CApplication::Update() 
{
	sf::Clock clock;
	while (m_mainWindow->isOpen())
	{
		float elapsed = clock.restart().asSeconds();
		sf::Event event;
		while (m_mainWindow->pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					m_mainWindow->close();
				break;
				case sf::Event::Resized:
				break;
				case sf::Event::GainedFocus:
				break;
				case sf::Event::LostFocus:
				break;
				default:
				break;
			}
			if (event.type == sf::Event::Closed)
			{
				m_mainWindow->close();
				break;
			}
		}
		m_keyboard->Update(elapsed);
		m_mouse->Update(elapsed);

		UpdateProject(elapsed);

		m_mainWindow->clear();
		// Draw
		m_mainWindow->display();
	}
}

void CApplication::Destroy() 
{
	DestroyProject( );
	m_renderer->Destroy();
	CSystems::DestroySystem<IRenderer>();
	m_keyboard->Destroy();
	CSystems::DestroySystem<Input::IKeyboard>();
}
