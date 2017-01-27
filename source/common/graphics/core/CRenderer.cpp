#include <LouEnginePrecompile.h>

#include <graphics/core/CRenderer.h>

#include <SFML/Window/Window.hpp>

CRenderer::CRenderer()
{

}

CRenderer::~CRenderer()
{

}

bool CRenderer::Init(const char *appTitle, int xRes, int yRes)
{
	sf::Window window(sf::VideoMode(xRes, yRes), appTitle);
	return true;
}

void CRenderer::Render()
{

}
