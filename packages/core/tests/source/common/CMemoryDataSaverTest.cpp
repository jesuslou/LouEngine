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

#include <memory/CMemoryDataProvider.h>
#include <memory/CMemoryDataSaver.h>

#include <gtest/gtest.h>

using SutDataType = int;

namespace MemoryDataProviderTestInternal
{
	const char* const FILE_NAME = "CMemoryDataSaverTest.txt";
	const SutDataType FIRST_VALUE = 1;
	const SutDataType SECOND_VALUE = 23;
	const std::size_t POD_ARRAY_N_VALUES = 4;
	const std::size_t BUFFER_SIZE = POD_ARRAY_N_VALUES * sizeof(SutDataType);
}

class CMemoryDataSaverTest : public ::testing::Test
{
public:
	CMemoryDataSaverTest()
	{
		m_buffer = static_cast<CMemoryDataProvider::u8*>(malloc(MemoryDataProviderTestInternal::BUFFER_SIZE));
	}

	~CMemoryDataSaverTest()
	{
		free(m_buffer);
	}
protected:
	CMemoryDataProvider::u8* m_buffer;
};

TEST_F(CMemoryDataSaverTest, test_memory_data_saver_no_memory_parentship)
{
	CMemoryDataSaver m_sut(m_buffer, MemoryDataProviderTestInternal::BUFFER_SIZE);
	ASSERT_FALSE(m_sut.IsParent());
	std::size_t capacity = m_sut.GetCapacity();
	ASSERT_EQ(capacity, MemoryDataProviderTestInternal::BUFFER_SIZE);
	std::size_t size = m_sut.GetSize();
	ASSERT_EQ(size, 0);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE);
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsParent());
	ASSERT_NE(m_buffer, nullptr);
}

TEST_F(CMemoryDataSaverTest, test_memory_data_saver_memory_parentship)
{
	CMemoryDataSaver m_sut(MemoryDataProviderTestInternal::BUFFER_SIZE);
	ASSERT_TRUE(m_sut.IsParent());
	std::size_t capacity = m_sut.GetCapacity();
	ASSERT_EQ(capacity, MemoryDataProviderTestInternal::BUFFER_SIZE);
	std::size_t size = m_sut.GetSize();
	ASSERT_EQ(size, 0);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE);
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsParent());
}

TEST_F(CMemoryDataSaverTest, test_memory_data_saver_write_POD)
{
	CMemoryDataSaver m_sut(MemoryDataProviderTestInternal::BUFFER_SIZE);
	m_sut.WritePOD(MemoryDataProviderTestInternal::FIRST_VALUE);
	std::size_t size = m_sut.GetSize();
	ASSERT_EQ(size, sizeof(SutDataType));
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE - sizeof(SutDataType));
	SutDataType secondValue = 0;
	m_sut.WritePOD(MemoryDataProviderTestInternal::SECOND_VALUE);
	size = m_sut.GetSize();
	ASSERT_EQ(size, sizeof(SutDataType) * 2);
	remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE - sizeof(SutDataType) * 2);
}

TEST_F(CMemoryDataSaverTest, test_memory_data_saver_rewind)
{
	CMemoryDataSaver m_sut(MemoryDataProviderTestInternal::BUFFER_SIZE);
	m_sut.WritePOD(MemoryDataProviderTestInternal::FIRST_VALUE);
	std::size_t size = m_sut.GetSize();
	ASSERT_EQ(size, sizeof(SutDataType));
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE - sizeof(SutDataType));
	m_sut.Rewind();
	size = m_sut.GetSize();
	ASSERT_EQ(size, 0);
	remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE);
}

TEST_F(CMemoryDataSaverTest, test_memory_data_saver_consume_POD)
{
	CMemoryDataSaver m_sut(m_buffer, MemoryDataProviderTestInternal::BUFFER_SIZE);
	int* value = m_sut.ConsumePOD<int>();
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE - sizeof(SutDataType));
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsValid());
	ASSERT_FALSE(m_sut.IsParent());
}

TEST_F(CMemoryDataSaverTest, test_memory_data_saver_consume_array)
{
	CMemoryDataSaver m_sut(m_buffer, MemoryDataProviderTestInternal::BUFFER_SIZE);
	int* values = m_sut.ConsumeArray<int>(MemoryDataProviderTestInternal::POD_ARRAY_N_VALUES);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, 0);
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsValid());
	ASSERT_FALSE(m_sut.IsParent());
}
