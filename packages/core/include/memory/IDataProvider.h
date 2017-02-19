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

class IDataProvider
{
public:
	enum class ESeekType
	{
		FromStart = SEEK_SET,
		FromCurrent = SEEK_CUR,
		FromEndOfFile = SEEK_END
	};

	IDataProvider() {}

	virtual bool IsValid() const = 0;
	virtual void Read(void *where, size_t nBytes) = 0;

	virtual std::size_t Seek(std::size_t offset, ESeekType from) = 0;
	virtual std::size_t Tell() const = 0;

	const char *GetName() const { return m_name.c_str(); }
	void SetName(const char *newName) { m_name = std::string(newName); }

	template< class T >
	void ReadPOD(T &data)
	{
		Read(&data, sizeof(T));
	}

	int ReadLong()
	{
		int x;
		ReadPOD(x);
		return x;
	}

	// Specialization for bool always read 4 bytes
	void ReadPOD(bool &data)
	{
		int x = ReadLong();
		data = (x != 0);
	}

protected:
	virtual ~IDataProvider() {}

	std::string m_name;
	FILE *m_file;
};
