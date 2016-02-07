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

#include "DataStore.h"

using namespace std;

DataStore::DataStore()
{
}

DataStore::~DataStore()
{
}

string DataStore::GetString(string key, string defaultValue)
{
	auto search = dataStrings.find(key);
	if (search != dataStrings.end())
	{
		return search->second;
	}
	else
	{
		return defaultValue;
	}
}

int DataStore::GetInt(string key, int defaultValue)
{
	auto search = dataInts.find(key);
	if (search != dataInts.end())
	{
		return search->second;
	}
	else
	{
		return defaultValue;
	}
}

bool DataStore::GetBool(string key, bool defaultValue)
{
	auto search = dataBools.find(key);
	if (search != dataBools.end())
	{
		return search->second;
	}
	else
	{
		return defaultValue;
	}
}

void DataStore::PutString(string key, string value)
{
	dataStrings[key] = value;
}

void DataStore::PutInt(string key, int value)
{
	dataInts[key] = value;
}

void DataStore::PutBool(string key, bool value)
{
	dataBools[key] = value;
}

map<string, string> DataStore::GetStringMap()
{
	return dataStrings;
}

map<string, int> DataStore::GetIntMap()
{
	return dataInts;
}

map<string, bool> DataStore::GetBoolMap()
{
	return dataBools;
}
