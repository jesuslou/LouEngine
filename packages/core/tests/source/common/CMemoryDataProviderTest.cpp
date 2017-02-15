#include <memory/CMemoryDataProvider.h>
#include <memory/CFileDataSaver.h>

#include <gtest/gtest.h>

using SutDataType = int;

namespace MemoryDataProviderTestInternal
{
	const char* const FILE_NAME = "CFileDataProviderTest.txt";
	const SutDataType FIRST_VALUE = 1;
	const SutDataType SECOND_VALUE = 23;
	const std::size_t POD_ARRAY_N_VALUES = 4;
	const std::size_t BUFFER_SIZE = POD_ARRAY_N_VALUES * sizeof(SutDataType);
}

class CMemoryDataProviderTest : public ::testing::Test
{
public:
	CMemoryDataProviderTest()
	{
		CFileDataSaver fds(MemoryDataProviderTestInternal::FILE_NAME);
		fds.WritePOD(MemoryDataProviderTestInternal::FIRST_VALUE);
		fds.WritePOD(MemoryDataProviderTestInternal::SECOND_VALUE);
		fds.Close();

		m_buffer = static_cast<CMemoryDataProvider::u8*>(malloc(MemoryDataProviderTestInternal::BUFFER_SIZE));
	}

	~CMemoryDataProviderTest()
	{
		free(m_buffer);
	}
protected:
	CMemoryDataProvider::u8* m_buffer;
};

TEST_F(CMemoryDataProviderTest, test_memory_data_provider_no_memory_ownership)
{
	CMemoryDataProvider m_sut(m_buffer, MemoryDataProviderTestInternal::BUFFER_SIZE);
	ASSERT_FALSE(m_sut.IsOwner());
	std::size_t size = m_sut.GetSize();
	ASSERT_EQ(size, MemoryDataProviderTestInternal::BUFFER_SIZE);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE);
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsOwner());
	ASSERT_NE(m_buffer, nullptr);
}

TEST_F(CMemoryDataProviderTest, test_memory_data_provider_memory_ownership)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::BUFFER_SIZE);
	ASSERT_TRUE(m_sut.IsOwner());
	std::size_t size = m_sut.GetSize();
	ASSERT_EQ(size, MemoryDataProviderTestInternal::BUFFER_SIZE);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE);
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsOwner());
}

TEST_F(CMemoryDataProviderTest, test_memory_data_provider_load_file)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	ASSERT_TRUE(m_sut.IsOwner());
	std::size_t size = m_sut.GetSize();
	ASSERT_EQ(size, sizeof(SutDataType) * 2);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, sizeof(SutDataType) + sizeof(MemoryDataProviderTestInternal::SECOND_VALUE));
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsOwner());
}

TEST_F(CMemoryDataProviderTest, test_memory_data_provider_read)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	SutDataType firstValue = 0;
	m_sut.Read(&firstValue, sizeof(SutDataType));
	ASSERT_EQ(firstValue, MemoryDataProviderTestInternal::FIRST_VALUE);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, sizeof(SutDataType));
	SutDataType secondValue = 0;
	m_sut.Read(&secondValue, sizeof(SutDataType));
	ASSERT_EQ(secondValue, MemoryDataProviderTestInternal::SECOND_VALUE);
	remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, 0);
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_read_long)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	SutDataType value = m_sut.ReadLong();
	ASSERT_EQ(value, MemoryDataProviderTestInternal::FIRST_VALUE);
	value = m_sut.ReadLong();
	ASSERT_EQ(value, MemoryDataProviderTestInternal::SECOND_VALUE);
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_read_POD)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	SutDataType value = 0;
	m_sut.ReadPOD(value);
	ASSERT_EQ(value, MemoryDataProviderTestInternal::FIRST_VALUE);
	m_sut.ReadPOD(value);
	ASSERT_EQ(value, MemoryDataProviderTestInternal::SECOND_VALUE);
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_seek_from_start)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	std::size_t newPos = m_sut.Seek(sizeof(SutDataType), IDataProvider::ESeekType::FROM_START);
	EXPECT_EQ(newPos, sizeof(SutDataType));
	int secondValue = m_sut.ReadLong();
	EXPECT_EQ(secondValue, MemoryDataProviderTestInternal::SECOND_VALUE);
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_seek_from_end_of_file)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	std::size_t newPos = m_sut.Seek(sizeof(SutDataType), IDataProvider::ESeekType::FROM_END_OF_FILE);
	EXPECT_EQ(newPos, m_sut.GetSize() - sizeof(SutDataType));
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_ftell)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	std::size_t newPos = m_sut.Seek(sizeof(SutDataType), IDataProvider::ESeekType::FROM_START);
	EXPECT_EQ(newPos, sizeof(SutDataType));
	std::size_t ftellPos = m_sut.Tell();
	EXPECT_EQ(ftellPos, newPos);
	EXPECT_EQ(ftellPos, sizeof(SutDataType));
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_rewind)
{
	CMemoryDataProvider m_sut(MemoryDataProviderTestInternal::FILE_NAME);
	std::size_t size = m_sut.GetSize();
	EXPECT_EQ(size, sizeof(SutDataType) * 2);
	SutDataType value = m_sut.ReadLong();
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, sizeof(SutDataType));
	value = m_sut.ReadLong();
	remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, 0);
	m_sut.Rewind();
	remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, sizeof(SutDataType) * 2);
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_assign_POD)
{
	CMemoryDataProvider m_sut(m_buffer, MemoryDataProviderTestInternal::BUFFER_SIZE);
	int* value = m_sut.AssignPOD<int>();
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, MemoryDataProviderTestInternal::BUFFER_SIZE - sizeof(SutDataType));
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsValid());
	ASSERT_FALSE(m_sut.IsOwner());

	m_sut.Load(MemoryDataProviderTestInternal::FILE_NAME);
	value = m_sut.AssignPOD<int>();
	ASSERT_EQ(*value, MemoryDataProviderTestInternal::FIRST_VALUE);
	remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, sizeof(SutDataType));
}

TEST_F(CMemoryDataProviderTest, test_file_data_provider_assign_POD_array)
{
	CMemoryDataProvider m_sut(m_buffer, MemoryDataProviderTestInternal::BUFFER_SIZE);
	int* values = m_sut.AssignPODArray<int>(MemoryDataProviderTestInternal::POD_ARRAY_N_VALUES);
	std::size_t remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, 0);
	m_sut.Destroy();
	ASSERT_FALSE(m_sut.IsValid());
	ASSERT_FALSE(m_sut.IsOwner());

	m_sut.Load(MemoryDataProviderTestInternal::FILE_NAME);
	values = m_sut.AssignPODArray<int>(2);
	ASSERT_EQ(values[0], MemoryDataProviderTestInternal::FIRST_VALUE);
	ASSERT_EQ(values[1], MemoryDataProviderTestInternal::SECOND_VALUE);
	remainingBytes = m_sut.GetRemainingBytes();
	ASSERT_EQ(remainingBytes, 0);
}
