#pragma once

class CGameSystems;
class IRenderer;

class CApplication
{
public:
	CApplication();

	bool Init(const char *appTitle, unsigned xRes, unsigned yRes);
	void Update();
	void Destroy();

protected:
	virtual ~CApplication() { }

	virtual bool InitProject() = 0;
	virtual void UpdateProject() = 0;
	virtual void DestroyProject() = 0;

	CGameSystems* m_gameSystems;
	IRenderer* m_renderer;
};
