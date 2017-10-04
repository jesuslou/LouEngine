#pragma once

#include <entities/components/IComponent.h>

struct SRotationComponent : public IComponent
{
	SRotationComponent() : m_angle(0.f) {}

	void ParseAtts() override
	{
		m_angle = 1.f;
	}

	float m_angle;
};
