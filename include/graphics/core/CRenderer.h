#pragma once

#include <graphics/core/IRenderer.h>

class CRenderer : public IRenderer
{
public:
	CRenderer();
	virtual ~CRenderer();

	bool Init(const char *appTitle, int xRes, int yRes) override;
	void Render() override;
};
