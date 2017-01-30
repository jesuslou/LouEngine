#pragma once

#include <application/SSFMLApplicationWindowParameters.h>

class CGameSystems;
class IRenderer;

namespace Input
{
	class IKeyboard;
}

namespace sf
{
	class RenderWindow;
}

class CSFMLApplication
{
public:
	CSFMLApplication();

	bool Init(const SSFMLApplicationWindowParameters& applicationWindowParameters = SSFMLApplicationWindowParameters());
	void Update();
	void Destroy();

protected:
	virtual ~CSFMLApplication();

	virtual bool InitProject(CGameSystems& gameSystems) = 0;
	virtual void UpdateProject() = 0;
	virtual void DestroyProject() = 0;

	CGameSystems* m_gameSystems;
	IRenderer* m_renderer;

	sf::RenderWindow* m_mainWindow;
	Input::IKeyboard* m_keyboard;
};
