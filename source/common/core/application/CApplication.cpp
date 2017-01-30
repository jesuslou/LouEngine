#include <LouEnginePrecompile.h>

#include <core/application/CApplication.h>
#include <core/systems/CSystems.h>
#include <graphics/core/CRenderer.h>

#include <SFML/Config.hpp>
#include <SFML/Window.hpp>

CApplication::CApplication()
	: m_gameSystems(nullptr)
	, m_renderer(nullptr)
{
}

CApplication::~CApplication()
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

bool CApplication::Init(const SApplicationWindowParameters& applicationWindowParameters/* = SApplicationWindowParameters()*/)
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

void CApplication::Update() 
{
	while (m_mainWindow->isOpen())
	{
		sf::Event event;
		while (m_mainWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_mainWindow->close();
		}
		UpdateProject();
	}
}

void CApplication::Destroy() 
{
	DestroyProject( );
	CSystems::DestroySystem<IRenderer>();
}
