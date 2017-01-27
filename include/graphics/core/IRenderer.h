#pragma once

class IRenderer
{
public:
	IRenderer() {}
	virtual ~IRenderer() {}

	virtual bool Init(const char *appTitle, int xRes, int yRes) = 0;
	virtual void Render() = 0;
};
