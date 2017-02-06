#include <LouEnginePrecompile.h>

#include <application/CApplication.h>
#include <application/SApplicationWindowParameters.h>
#include <input/CKeyboard.h>
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

		UpdateProject();

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
