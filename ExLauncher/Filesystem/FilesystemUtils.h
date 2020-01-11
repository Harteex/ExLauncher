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

#ifndef _FILESYSTEM_UTILS_H
#define _FILESYSTEM_UTILS_H

/*********************************************/

#include <string>
#include <vector>

/*********************************************/

namespace FilesystemUtils
{
	std::vector<std::string> GetDirectories(std::string path);
	std::vector<std::string> GetFilesByMimeTypes(std::string path, std::vector<std::string> mimeTypes);
	std::vector<std::string> GetFilesByExtension(std::string path, std::string extension);
	std::string GetFilename(std::string path, bool withExtension = true);
}

#endif
