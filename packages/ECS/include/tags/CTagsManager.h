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

#include <hash/CStrID.h>

#include <bitset>
#include <map>

using TagsMask = std::bitset<MAX_TAGS>;

class CTagsManager
{
public:
	bool RegisterTag(CStrID tag);
	int GetTagIdx(CStrID tag) const;
	bool SetTag(TagsMask& mask, CStrID tag, bool add);

	bool HasTag(const TagsMask& mask, CStrID tag) const;

	template<typename... Args>
	bool AddTags(TagsMask& mask, Args... args)
	{
		return AddTagsInternal(mask, CStrID(args)...);
	}

	template<typename... Args>
	bool RemoveTags(TagsMask& mask, Args... args)
	{
		return RemoveTagsInternal(mask, CStrID(args)...);
	}

	template<typename... Args>
	TagsMask GetTagMask(Args... args)
	{
		TagsMask mask;
		GetTagMaskInternal(mask, CStrID(args)...);
		return mask;
	}

	template<typename... Args>
	bool HasAllTags(const TagsMask& mask, Args... args) const
	{
		return HasAllTagsInternal(mask, CStrID(args)...);
	}

	template<typename... Args>
	bool HasAnyTags(const TagsMask& mask, Args... args) const
	{
		return HasAnyTagsInternal(mask, CStrID(args)...);
	}

private:
	template<typename... Args>
	bool AddTagsInternal(TagsMask& mask, CStrID tag, Args... args)
	{
		bool success = SetTag(mask, tag, true);
		return AddTagsInternal(mask, args...) && success;
	}

	bool AddTagsInternal(TagsMask& mask, CStrID tag)
	{
		return SetTag(mask, tag, true);
	}

	template<typename... Args>
	bool RemoveTagsInternal(TagsMask& mask, CStrID tag, Args... args)
	{
		bool success = SetTag(mask, tag, false);
		return RemoveTagsInternal(mask, args...) && success;
	}

	bool RemoveTagsInternal(TagsMask& mask, CStrID tag)
	{
		return SetTag(mask, tag, false);
	}

	template<typename... Args>
	void GetTagMaskInternal(TagsMask& mask, CStrID tag, Args... args)
	{
		SetTag(mask, tag, true);
		GetTagMaskInternal(mask, args...);
	}

	void GetTagMaskInternal(TagsMask& mask, CStrID tag)
	{
		SetTag(mask, tag, true);
	}

	template<typename... Args>
	bool HasAllTagsInternal(const TagsMask& mask, CStrID tag, Args... args) const
	{
		return HasAllTagsInternal(mask, args...) && mask.test(GetTagIdx(tag));
	}

	bool HasAllTagsInternal(const TagsMask& mask, CStrID tag) const
	{
		return HasTag(mask, tag);
	}

	template<typename... Args>
	bool HasAnyTagsInternal(const TagsMask& mask, CStrID tag, Args... args) const
	{
		return HasAnyTagsInternal(mask, args...) || mask.test(GetTagIdx(tag));
	}

	bool HasAnyTagsInternal(const TagsMask& mask, CStrID tag) const
	{
		return HasTag(mask, tag);
	}

	std::map<CStrID, int> m_registeredTags;
};
