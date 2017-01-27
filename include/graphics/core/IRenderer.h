#pragma once

class IRenderer
{
public:
	IRenderer() {}
	virtual ~IRenderer() {}

	virtual bool Init() = 0;
	virtual void Render() = 0;
};
