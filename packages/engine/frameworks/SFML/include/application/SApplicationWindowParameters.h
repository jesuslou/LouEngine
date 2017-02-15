////////////////////////////////////////////////////////////
//
// LouEngine - Multiplatform Game Engine Project
// Copyright (C) 2016-2017 Jesus Lou (loucocito@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

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
