/*
Copyright 2020 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "FilesystemUtils.h"
#include "../utils.h"
#include "../global.h"
#include <stdio.h>
#include <algorithm>
#ifdef WINDOWS
#include <windows.h>
#include <tchar.h>
#endif
#ifdef UNIX
#include <dirent.h>
#endif

#ifdef HAS_LIBXDGMIME
#include <xdgmime.h>
#endif

using namespace std;

vector<string> FilesystemUtils::GetDirectories(string path)
{
	vector<string> directories;

#ifdef WINDOWS
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	string pathMatch = path + "*";

	hFind = FindFirstFile(pathMatch.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (string(".") != ffd.cFileName && string("..") != ffd.cFileName)
					directories.push_back(ffd.cFileName);
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);
	}

#else
	DIR* dirp = opendir(path.c_str());
	if (dirp != nullptr)
	{
		struct dirent* entry;
		while ((entry = readdir(dirp)) != NULL)
		{
			if (entry->d_type == DT_DIR)
			{
				if (string(".") != entry->d_name && string("..") != entry->d_name)
					directories.push_back(entry->d_name);
			}
		}

		closedir(dirp);
		sort(directories.begin(), directories.end());
	}
#endif

	return directories;
}

bool FileEndsWithExtension(string file, vector<string> extensions)
{
	for (string extension : extensions)
	{
		if (stringEndsWith(file, extension))
			return true;
	}

	return false;
}

vector<string> FilesystemUtils::GetFilesByMimeTypes(string path, vector<string> mimeTypes)
{
	vector<string> files = GetFilesByExtension(path, "");

	if (mimeTypes.empty())
		return files;

	vector<string> filteredFiles;
	
#ifdef HAS_LIBXDGMIME

	vector<string> extensions;

	for (string mimeType : mimeTypes)
	{
		int numExt = 16;
		char* extensionsArr[16];
		numExt = xdg_mime_get_extensions_from_mime_type(mimeType.c_str(), extensionsArr, numExt);

		for (int i = 0; i < numExt; i++)
		{
			extensions.push_back((string)extensionsArr[i]);
			free(extensionsArr[i]);
		}
	}

	for (string file : files)
	{
		if (FileEndsWithExtension(file, extensions))
			filteredFiles.push_back(file);
	}

#else
	filteredFiles = files;
#endif

	return filteredFiles;
}

vector<string> FilesystemUtils::GetFilesByExtension(string path, string extension)
{
	vector<string> files;

#ifdef WINDOWS
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	string pathMatch = path;
	pathMatch += "*";
	if (!extension.empty())
	{
		pathMatch += ".";
		pathMatch += extension;
	}

	hFind = FindFirstFile(pathMatch.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL ||
				ffd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ||
				ffd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED ||
				ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			{
				if (string(".") != ffd.cFileName && string("..") != ffd.cFileName)
					files.push_back(ffd.cFileName);
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);
	}

#else
	DIR* dirp = opendir(path.c_str());
	if (dirp != nullptr)
	{
		struct dirent* entry;
		while ((entry = readdir(dirp)) != NULL)
		{
			if (entry->d_type == DT_REG && string(".") != entry->d_name && string("..") != entry->d_name)
			{
				if (extension.empty() || stringEndsWith(entry->d_name, string(".") + extension))
					files.push_back(entry->d_name);
			}
		}

		closedir(dirp);
		sort(files.begin(), files.end());
	}
#endif

	return files;
}

std::string FilesystemUtils::GetFilename(std::string path, bool withExtension)
{
	auto found = path.find_last_of("/\\");
	if (found == std::string::npos)
		return "";

	string filename = path.substr(found + 1);

	if (!withExtension)
	{
		auto foundExt = filename.find_last_of(".");
		if (foundExt != std::string::npos)
		{
			filename = filename.substr(0, foundExt);
		}
	}

	return filename;
}