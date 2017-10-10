#pragma once

#include <component/CComponent.h>

class CCompCamera : public CComponent
{
public:
	CCompCamera() : p(20) {}

private:
	int p;
};
