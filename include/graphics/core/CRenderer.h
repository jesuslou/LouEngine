#pragma once

#include <graphics/core/IRenderer.h>

class CRenderer : public IRenderer
{
public:
	CRenderer();
	virtual ~CRenderer();

	bool Init() override;
	void Render() override;
};
