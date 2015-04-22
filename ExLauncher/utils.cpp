#include "utils.h"

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

// FIXME find some good solution here..
std::string trimRight(std::string str, const std::string& trimChars)
{
   std::string::size_type pos = str.find_last_not_of(trimChars);
   str.erase(pos + 1);
   return str;
}

std::string trimLeft(std::string str, const std::string& trimChars)
{
   std::string::size_type pos = str.find_first_not_of(trimChars);
   str.erase(0, pos);
   return str;
}

std::string trim(std::string str, const std::string& trimChars)
{
	return trimRight(trimLeft(str, trimChars), trimChars);
}

string lineBreak(string text, int lineLength)
{
	string out = "";

    int curRowStartPos = 0;
    int curRowLength = 0;
    int lastSpaceForCurRow = -1;
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
			out += trim(text.substr(curRowStartPos, i - curRowStartPos));
			out += '\n';
            lastSpaceForCurRow = -1;
            curRowLength = 0;
            curRowStartPos = i + 1;
            continue;
        }

        if (text[i] == ' ')
        {
            lastSpaceForCurRow = i;
        }

        if (curRowLength >= lineLength)
        {
            // If we have an appropriate space to break on, do so, otherwise force break anyway
            if (lastSpaceForCurRow > -1)
            {
                i = lastSpaceForCurRow;
            }

			out += trim(text.substr(curRowStartPos, i - curRowStartPos));
			out += '\n';
            lastSpaceForCurRow = -1;
            curRowLength = 0;
            curRowStartPos = i;
            continue;
        }

        curRowLength++;
    }

    if (text.length() > curRowStartPos)
    {
		out += trim(text.substr(curRowStartPos, text.length() - curRowStartPos));
    }

    return out;
}

int clip(int val, int min, int max)
{
	if (val < min)
		return min;

	if (val > max)
		return max;

	return val;
}

double clip(double val, double min, double max)
{
	if (val < min)
		return min;

	if (val > max)
		return max;

	return val;
}
