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

#include "memory/IDataProvider.h"

class CFileDataProvider : public IDataProvider
{
public:
	static bool FileExist(const char* fileName);
	static std::size_t GetFileSize(const char* fileName);

	CFileDataProvider();
	CFileDataProvider(const char* fileName);
	~CFileDataProvider();

	void Read(void* where, std::size_t nBytes) override;
	bool IsValid() const override;

	bool Open(const char* fileName);
	void Close();

	std::size_t Seek(std::size_t offset, ESeekType from) override;
	std::size_t Tell() const override;

	std::size_t GetSize() const;

private:
	std::size_t m_startOffset;
	std::size_t m_maxBytes;
};
