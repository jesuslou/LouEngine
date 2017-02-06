#pragma once

#include <application\IApplication.h>

struct SDL_Window;

class CApplication : public IApplication
{
public:
	CApplication();

	bool Init(const SApplicationWindowParameters& applicationWindowParameters) override;
	void Update() override;
	void Destroy() override;

protected:
	virtual ~CApplication();

	bool InitSDL(const SApplicationWindowParameters& applicationWindowParameters);

	SDL_Window* m_window;
};
