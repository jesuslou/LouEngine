#pragma once
#include <systems/CSystems.h>

struct SApplicationWindowParameters;
class IRenderer;

namespace Input
{
	class IKeyboard;
	class IMouse;
}

class IApplication
{
public:
	IApplication() 
		: m_renderer(nullptr)
		, m_keyboard(nullptr)
		, m_mouse(nullptr)
	{
		CSystems::SetGameSystems(&m_gameSystems);
	}
	virtual ~IApplication() {}

	virtual bool Init(const SApplicationWindowParameters& applicationWindowParameters) = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

protected:
	virtual bool InitProject(CGameSystems& gameSystems) = 0;
	virtual void UpdateProject() = 0;
	virtual void DestroyProject() = 0;

	CGameSystems m_gameSystems;

	IRenderer* m_renderer;
	Input::IKeyboard* m_keyboard;
	Input::IMouse* m_mouse;
};
