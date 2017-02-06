#pragma once

#include <string>

struct SApplicationWindowParameters
{
	SApplicationWindowParameters(int xRes = 800
								 , int yRes = 600
								 , const std::string& windowTitle = "Empty Project"
								 , bool hasTitleBar = true
								 , bool hasCloseButton = true
								 , bool hasResizeButton = true
								 , bool isFullScreen = false
								 , bool hasVerticalSync = true
								 , int frameRateLimit = 60
								 )
		: m_xRes(xRes)
		, m_yRes(yRes)
		, m_windowTitle(windowTitle)
		, m_hasTitleBar(hasTitleBar)
		, m_hasCloseButton(hasCloseButton)
		, m_hasResizeButton(hasResizeButton)
		, m_isFullScreen(isFullScreen)
		, m_hasVerticalSync(hasVerticalSync)
		, m_frameRateLimit(frameRateLimit)
	{
	}

	int m_xRes;
	int m_yRes;
	std::string m_windowTitle;
	bool m_hasTitleBar;
	bool m_hasCloseButton;
	bool m_hasResizeButton;
	bool m_isFullScreen;
	bool m_hasVerticalSync;
	int m_frameRateLimit;
};
