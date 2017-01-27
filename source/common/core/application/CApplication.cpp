#include <LouEnginePrecompile.h>

#include <core/application/CApplication.h>
#include <core/systems/CSystems.h>
#include <graphics/core/CRenderer.h>

#include <SFML/Window.hpp>

CApplication::CApplication( ) 
{ 
}

bool CApplication::Init(const char *appTitle, unsigned xRes, unsigned yRes) {
	printf("Project '%s' initialized\n", appTitle);

	m_gameSystems = new CGameSystems();
	CSystems::SetGameSystems(m_gameSystems);

	m_renderer = new CRenderer();
	if (!m_renderer->Init(appTitle, xRes, yRes))
	{
		delete m_renderer;
		return false;
	}
	CSystems::SetSystem<IRenderer>(m_renderer);

  return InitProject( );
}

void CApplication::Update( ) {
	printf("Updating...\n");
	UpdateProject();
	getchar();
}

void CApplication::Destroy( ) {
	printf("Destroying...\n");
	DestroyProject( );
	delete m_gameSystems;
	getchar();
}
