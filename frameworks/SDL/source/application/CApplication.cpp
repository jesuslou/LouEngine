#include <LouEnginePrecompile.h>

#include <application/CApplication.h>
#include <application/SApplicationWindowParameters.h>
#include <input/CKeyboard.h>
#include <input/CMouse.h>
#include <graphics/CRenderer.h>
#include <systems/CSystems.h>

#include <SDL.h>

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

	m_mouse = new Input::CMouse();
	if (!m_mouse->Init())
	{
		delete m_mouse;
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
				break;
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
	DestroyProject( );
	m_renderer->Destroy();
	CSystems::DestroySystem<IRenderer>();
	m_keyboard->Destroy();
	CSystems::DestroySystem<Input::IKeyboard>();
}

bool CApplication::InitSDL(const SApplicationWindowParameters& applicationWindowParameters)
{
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
			//printf("CRenderer::SDL Window creation SUCCESS!\n");
			//m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			//// Project specific initialization
			//if (m_renderer == nullptr)
			//{
			//	printf("CRenderer::Renderer creation FAILURE! SDL Error: %s\n", SDL_GetError());
			//	return false;
			//}
			//else
			//{
			//	printf("CRenderer::SDL Renderer creation SUCCESS!\n");
			//	//Initialize renderer color
			//	SDL_SetRenderDrawColor(m_renderer, m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
			//}
		}
	}
	return true;
}
