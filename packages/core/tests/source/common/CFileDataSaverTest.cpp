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

#include <memory/CFileDataProvider.h>
#include <memory/CFileDataSaver.h>

#include <gtest/gtest.h>

using SutDataType = int;

namespace FileDataSaverTestInternal
{
	const char* const FILE_NAME = "CFileDataSaverTest.txt";
	const SutDataType FIRST_VALUE = 1;
	const SutDataType SECOND_VALUE = 23;
}

class CFileDataSaverTest : public ::testing::Test
{
public:
	CFileDataSaverTest()
	{
	}

	~CFileDataSaverTest()
	{
	}
};

TEST_F(CFileDataSaverTest, test_file_data_saver_open)
{
	CFileDataSaver m_sut;
	m_sut.Open(FileDataSaverTestInternal::FILE_NAME);
	EXPECT_TRUE(m_sut.IsValid());
}

TEST_F(CFileDataSaverTest, test_file_data_saver_close)
{
	CFileDataSaver m_sut;
	m_sut.Open(FileDataSaverTestInternal::FILE_NAME);
	EXPECT_TRUE(m_sut.IsValid());
	m_sut.Close();
	EXPECT_FALSE(m_sut.IsValid());
}

TEST_F(CFileDataSaverTest, test_file_data_saver_write_POD)
{
	CFileDataSaver m_sut(FileDataSaverTestInternal::FILE_NAME);

	m_sut.WritePOD(FileDataSaverTestInternal::FIRST_VALUE);
	m_sut.WritePOD(FileDataSaverTestInternal::SECOND_VALUE);
	m_sut.Close();
	EXPECT_FALSE(m_sut.IsValid());

	CFileDataProvider fdp(FileDataSaverTestInternal::FILE_NAME);
	EXPECT_TRUE(fdp.IsValid());
	SutDataType value = 0;
	fdp.ReadPOD(value);
	EXPECT_EQ(value, FileDataSaverTestInternal::FIRST_VALUE);
	fdp.ReadPOD(value);
	EXPECT_EQ(value, FileDataSaverTestInternal::SECOND_VALUE);
	fdp.Close();
	EXPECT_FALSE(fdp.IsValid());
}
