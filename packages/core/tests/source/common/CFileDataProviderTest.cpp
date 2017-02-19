#include <memory/CFileDataProvider.h>
#include <memory/CFileDataSaver.h>

#include <gtest/gtest.h>

using SutDataType = int;

namespace FileDataProviderTestInternal
{
	const char* const FILE_NAME = "CFileDataProviderTest.txt";
	const SutDataType FIRST_VALUE = 1;
	const SutDataType SECOND_VALUE = 23;
}

class CFileDataProviderTest : public ::testing::Test
{
public:
	CFileDataProviderTest()
	{
		CFileDataSaver fds(FileDataProviderTestInternal::FILE_NAME);
		fds.WritePOD(FileDataProviderTestInternal::FIRST_VALUE);
		fds.WritePOD(FileDataProviderTestInternal::SECOND_VALUE);
		fds.Close();
	}

	~CFileDataProviderTest()
	{
	}
};

TEST_F(CFileDataProviderTest, test_file_exists)
{
	bool exists = CFileDataProvider::FileExist(__FILE__);
	EXPECT_TRUE(exists);
}

TEST_F(CFileDataProviderTest, test_get_file_size)
{
	std::size_t size = CFileDataProvider::GetFileSize(FileDataProviderTestInternal::FILE_NAME);
	EXPECT_EQ(size, sizeof(SutDataType) * 2);
}

TEST_F(CFileDataProviderTest, test_file_data_provider_open)
{
	CFileDataProvider m_sut;
	m_sut.Open(FileDataProviderTestInternal::FILE_NAME);
	EXPECT_TRUE(m_sut.IsValid());
}

TEST_F(CFileDataProviderTest, test_file_data_provider_read_long)
{
	CFileDataProvider m_sut(FileDataProviderTestInternal::FILE_NAME);
	SutDataType firstValue = m_sut.ReadLong();
	EXPECT_EQ(firstValue, FileDataProviderTestInternal::FIRST_VALUE);
	SutDataType secondValue = m_sut.ReadLong();
	EXPECT_EQ(secondValue, FileDataProviderTestInternal::SECOND_VALUE);
}

TEST_F(CFileDataProviderTest, test_file_data_provider_get_size)
{
	CFileDataProvider m_sut(FileDataProviderTestInternal::FILE_NAME);
	std::size_t size = m_sut.GetSize();
	EXPECT_EQ(size, sizeof(SutDataType) + sizeof(SutDataType));
}

TEST_F(CFileDataProviderTest, test_file_data_provider_seek_from_start)
{
	CFileDataProvider m_sut(FileDataProviderTestInternal::FILE_NAME);
	std::size_t newPos = m_sut.Seek(sizeof(SutDataType), IDataProvider::ESeekType::FromStart);
	EXPECT_EQ(newPos, sizeof(SutDataType));
	SutDataType secondValue = m_sut.ReadLong();
	EXPECT_EQ(secondValue, FileDataProviderTestInternal::SECOND_VALUE);
}

TEST_F(CFileDataProviderTest, test_file_data_provider_seek_from_end_of_file)
{
	CFileDataProvider m_sut(FileDataProviderTestInternal::FILE_NAME);
	std::size_t newPos = m_sut.Seek(sizeof(SutDataType), IDataProvider::ESeekType::FromEndOfFile);
	EXPECT_EQ(newPos, m_sut.GetSize() + sizeof(SutDataType));
}

TEST_F(CFileDataProviderTest, test_file_data_provider_ftell)
{
	CFileDataProvider m_sut(FileDataProviderTestInternal::FILE_NAME);
	std::size_t newPos = m_sut.Seek(sizeof(SutDataType), IDataProvider::ESeekType::FromStart);
	EXPECT_EQ(newPos, sizeof(SutDataType));
	std::size_t ftellPos = m_sut.Tell();
	EXPECT_EQ(ftellPos, newPos);
	EXPECT_EQ(ftellPos, sizeof(SutDataType));
}
