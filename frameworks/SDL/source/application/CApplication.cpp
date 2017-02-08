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
	//Event handler
	SDL_Event event;

	//Set text color as black
	SDL_Color textColor = { 0, 0, 0, 255 };

	//While application is running
	while (true)
	{
		//CMicroTimer timer;
		//timer.start();

		//CInputManager::get().update();

		//Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			//User requests quit
			if (event.type == SDL_QUIT)
			{
				break;
			}

			Input::CMouse* mouse = static_cast<Input::CMouse*>(m_mouse);
			mouse->updateSDLMouse(event);
		}
		
		CVector2i mousePos = m_mouse->GetMouseGlobalPosition();
		printf("mousePos: %d - %d\n", mousePos.x, mousePos.y);

		m_keyboard->Update(0.016f);

		UpdateProject();

		//m_elapsed = timer.elapsed();
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
