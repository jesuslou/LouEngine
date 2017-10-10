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

class CEntity;

class CHandle
{
public:
	static constexpr int MAX_ELEMENT_TYPES = 4;
	static constexpr int MAX_COMPONENT_TYPES = 512;
	static constexpr int MAX_ELEMENTS = 8192;
	static constexpr int MAX_VERSIONS = 256;

	CHandle()
		: m_elementType(0)
		, m_componentIdx(0)
		, m_elementPosition(0)
		, m_version(0)
	{}
	CHandle(CEntity* rhs);

	const CHandle & operator=(CEntity* rhs);
	operator CEntity*();
	operator bool();

//private:
	unsigned m_elementType : 2; //  4
	unsigned m_componentIdx : 9; // 512
	unsigned m_elementPosition : 13; // 8.192
	unsigned m_version : 8; // 256
};