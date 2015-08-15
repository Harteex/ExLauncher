#include "App.h"

using namespace std;

App::App()
{
	//icon = NULL;
}

map<string, string>& App::GetAllData()
{
	return data;
}

string App::GetData(string key, string defaultValue)
{
	auto search = data.find(key);
	if (search != data.end())
	{
		return search->second;
	}
	else
	{
		return defaultValue;
	}
}

void App::SetData(string key, string value)
{
	data[key] = value;
}
