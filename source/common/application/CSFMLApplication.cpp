#include <LouEnginePrecompile.h>

#include <application/CSFMLApplication.h>
#include <systems/CSystems.h>
#include <input/CSFMLKeyboard.h>
#include <graphics/CRenderer.h>

#include <SFML/Config.hpp>
#include <SFML/Window.hpp>

CSFMLApplication::CSFMLApplication()
	: m_gameSystems(nullptr)
	, m_renderer(nullptr)
	, m_keyboard(nullptr)
{
}

CSFMLApplication::~CSFMLApplication()
{
	if (m_gameSystems)
	{
		delete m_gameSystems;
		m_gameSystems = nullptr;
	}

	if (m_mainWindow)
	{
		delete m_mainWindow;
		m_mainWindow = nullptr;
	}
}

bool CSFMLApplication::Init(const SSFMLApplicationWindowParameters& applicationWindowParameters/* = SSFMLApplicationWindowParameters()*/)
{
	m_gameSystems = new CGameSystems();
	CSystems::SetGameSystems(m_gameSystems);

	m_renderer = new CRenderer();
	if (!m_renderer->Init())
	{
		delete m_renderer;
		return false;
	}
	CSystems::SetSystem<IRenderer>(m_renderer);

	m_keyboard = new Input::CSFMLKeyboard();
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

	m_mainWindow = new sf::Window(sf::VideoMode(applicationWindowParameters.m_xRes, applicationWindowParameters.m_yRes)
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

	return InitProject(*m_gameSystems);
}

void CSFMLApplication::Update() 
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
	}
}

void CSFMLApplication::Destroy() 
{
	DestroyProject( );
	m_renderer->Destroy();
	CSystems::DestroySystem<IRenderer>();
	m_keyboard->Destroy();
	CSystems::DestroySystem<Input::IKeyboard>();
}
