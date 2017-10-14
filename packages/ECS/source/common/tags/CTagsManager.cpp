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

#include <tags/CTagsManager.h>

bool CTagsManager::RegisterTag(CStrID tag)
{
	if (m_registeredTags.find(tag) == m_registeredTags.end())
	{
		m_registeredTags[tag] = m_registeredTags.size();
		return true;
	}
	return false;
}

int CTagsManager::GetTagIdx(CStrID tag) const
{
	auto tagItr = m_registeredTags.find(tag);
	if (tagItr != m_registeredTags.end())
	{
		return tagItr->second;
	}
	return -1;
}

bool CTagsManager::SetTag(TagsMask& mask, CStrID tag, bool add)
{
	int tagIdx = GetTagIdx(tag);
	if (tagIdx >= 0)
	{
		mask[tagIdx] = add;
		return true;
	}
	return false;
}


bool CTagsManager::HasTag(const TagsMask& mask, CStrID tag) const
{
	int tagIdx = GetTagIdx(tag);
	return tagIdx >= 0 ? mask.test(tagIdx) : false;
}
