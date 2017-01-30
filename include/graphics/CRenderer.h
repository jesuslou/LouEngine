#pragma once

#include <graphics/IRenderer.h>

class CRenderer : public IRenderer
{
public:
	CRenderer();
	~CRenderer();

	bool Init() override;
	void Destroy() override;
	void Render() override;
};
