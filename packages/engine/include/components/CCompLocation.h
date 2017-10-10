#pragma once

#include <component/CComponent.h>

class CCompLocation : public CComponent
{
public:
	CCompLocation() : p(10) {}

private:
	int p;
};
