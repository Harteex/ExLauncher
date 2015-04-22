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
