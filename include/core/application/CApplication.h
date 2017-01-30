#pragma once

#include <core/application/SApplicationWindowParameters.h>

class CGameSystems;
class IRenderer;

namespace sf
{
	class Window;
}

class CApplication
{
public:
	CApplication();

	bool Init(const SApplicationWindowParameters& applicationWindowParameters = SApplicationWindowParameters());
	void Update();
	void Destroy();

protected:
	virtual ~CApplication();

	virtual bool InitProject(CGameSystems& gameSystems) = 0;
	virtual void UpdateProject() = 0;
	virtual void DestroyProject() = 0;

	CGameSystems* m_gameSystems;
	IRenderer* m_renderer;

	sf::Window* m_mainWindow;
};
