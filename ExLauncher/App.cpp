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

void App::SetData(string key, string value)
{
	data[key] = value;
}
