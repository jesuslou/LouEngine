#pragma once

class CApplication
{
public:
	CApplication();

	bool Init(const char *appTitle, unsigned xRes, unsigned yRes);
	void Update();
	void Destroy();

protected:
	virtual bool InitProject() = 0;
	virtual void DestroyProject() = 0;
};
