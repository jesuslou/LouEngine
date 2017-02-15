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

#include <sstream>

namespace StringUtils
{
	static bool BeginsWith(const std::string& source, const std::string& otherText)
	{
		return strncmp(source.c_str(), otherText.c_str(), otherText.length()) == 0;
	}

	static bool EndsWith(const std::string& source, const std::string& otherText)
	{
		size_t other_nchars = otherText.length();
		size_t my_nchars = source.length();
		if (other_nchars > my_nchars)
			return false;
		return strcmp(source.c_str() + my_nchars - other_nchars, otherText.c_str()) == 0;
	}

	// trim from start
	static bool TrimLeft(std::string& source, const std::string& pattern)
	{
		if (BeginsWith(source, pattern))
		{
			source = std::string(source.c_str() + pattern.length());
			return true;
		}
		return false;
	}

	// trim from end
	static bool TrimRight(std::string& source, const std::string& pattern)
	{
		if (EndsWith(source, pattern))
		{
			const char* sourceCStr = source.c_str();
			source[source.length() - pattern.length()] = '\0';
			return true;
		}
		return false;
	}

	template<typename T, typename... Args>
	static std::string Format(T firstArg, Args... others)
	{
		std::ostringstream os;
		FormatInternal(os, firstArg, others...);
		return os.str();
	}

	template<typename T, typename... Args>
	static void FormatInternal(std::ostringstream& os, T firstArg, Args... others)
	{
		os << firstArg;
		FormatInternal(os, others...);
	}
	template<typename T>
	static void FormatInternal(std::ostringstream& os, T lastArg)
	{
		os << lastArg;
	}
}
