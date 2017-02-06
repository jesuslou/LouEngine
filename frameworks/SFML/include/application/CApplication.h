#pragma once

#include <application\IApplication.h>

namespace sf
{
	class RenderWindow;
}

class CApplication : public IApplication
{
public:
	CApplication();

	bool Init(const SApplicationWindowParameters& applicationWindowParameters) override;
	void Update() override;
	void Destroy() override;

protected:
	virtual ~CApplication();

	sf::RenderWindow* m_mainWindow;
};
