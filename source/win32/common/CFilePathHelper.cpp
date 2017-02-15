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

#include <LouEnginePrecompile.h>

#include <common/CFilePathHelper.h>
#include <common/StringUtils.h>

#include <windows.h>
#include <stdarg.h>
#include <minwinbase.h>
#include <fileapi.h>
#include <handleapi.h>

CFilePathHelper::CFilePathHelper(const char* basePath)
	: m_basePath(basePath)
{
}

bool CFilePathHelper::FolderExists(const char *folderName)
{
	std::string searchPath = StringUtils::Format(folderName, "\\*.*");

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && !strcmp(fd.cFileName, folderName))
			{
				return true;
			}
		}
		while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return false;
}

std::string CFilePathHelper::getFileExtension(const std::string &filename)
{
	for (int i = filename.size() - 1; i >= 0; --i)
	{
		if (filename[i] == '.')
		{
			std::string extension = filename.substr(i + 1);
			return extension;
		}
	}
	return nullptr;
}

std::string CFilePathHelper::getFileName(const std::string &fileName)
{
	std::string path = getFolderName(fileName);
	std::string ext = getFileExtension(fileName);
	std::string file(path.c_str());

	//file.replace()
	//StringUtils::TrimRight()
	//file.trimRight(Format(".%s", ext.c_str()));

	return std::string(file.c_str());
}

std::string CFilePathHelper::getFolderName(const std::string &path)
{
	unsigned n_slash = 0;
	for (int i = path.size() - 1; i >= 0; --i)
	{
		if (path[i] == '\\' || path[i] == '/')
		{
			if (i != path.size() - 1)
			{
				std::string name = "";
				name = (n_slash > 0 ? path.substr(i + 1, path.size() - i - 2) : path.substr(i + 1));
				return name;
			}
			else
			{
				++n_slash;
			}
		}
	}
	return nullptr;
}

std::vector<std::string> CFilePathHelper::GatherAvailableFilesWithAbsolutePath() const
{
	std::string searchPath = StringUtils::Format(m_basePath.c_str(), "\\*.*");
	std::vector<std::string> allFiles;

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				allFiles.push_back(StringUtils::Format(m_basePath.c_str(), "/", fd.cFileName).c_str());
			}
		}
		while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return allFiles;
}

std::vector<std::string> CFilePathHelper::GatherAvailableFileNames() const
{
	std::string searchPath = StringUtils::Format(m_basePath.c_str(), "\\*.*");
	std::vector<std::string> allFiles;

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				allFiles.push_back(fd.cFileName);
			}
		}
		while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return allFiles;
}

std::vector<std::string> CFilePathHelper::GatherAvailableFoldersWithAbsolutePath() const
{
	std::string searchPath = StringUtils::Format(m_basePath.c_str(), "\\*.*");
	std::vector<std::string> allFolders;

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
				strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
			{
				allFolders.push_back(StringUtils::Format(m_basePath.c_str(), "/", fd.cFileName).c_str());
			}
		}
		while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return allFolders;
}

std::vector<std::string> CFilePathHelper::GatherAvailableFolders() const
{
	std::string searchPath = StringUtils::Format(m_basePath.c_str(), "\\*.*");
	std::vector<std::string> allFolders;

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
				strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
			{
				allFolders.push_back(fd.cFileName);
			}
		}
		while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return allFolders;
}
