/*
Copyright 2016 Andreas Bjerkeholt

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

#include <string>
#include <sstream>
#include <vector>

const std::string whiteSpaces(" \f\n\r\t\v");

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
/*std::string trimRight(std::string& str, const std::string& trimChars = whiteSpaces);
std::string trimLeft(std::string& str, const std::string& trimChars = whiteSpaces);
std::string trim(std::string& str, const std::string& trimChars = whiteSpaces);*/
std::string trimRight(std::string str, const std::string& trimChars = whiteSpaces);
std::string trimLeft(std::string str, const std::string& trimChars = whiteSpaces);
std::string trim(std::string str, const std::string& trimChars = whiteSpaces);
std::string lineBreak(std::string text, int lineLength);
int clip(int val, int min, int max);
double clip(double val, double min, double max);
void measureTimeStart();
double measureTimeFinish();
std::vector<std::string> getDirectories(std::string path);
std::vector<std::string> getFilesByExtension(std::string path, std::string extension);
std::string getFilename(std::string path, bool withExtension = true);
std::string getCapitalizedString(std::string str);
bool stringEndsWith(std::string const& str, std::string const& end);
