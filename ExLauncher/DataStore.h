#ifndef _DATA_STORE_H_
#define _DATA_STORE_H_

/*********************************************/

#include <string>
#include <map>

/*********************************************/

class DataStore
{
private:
	std::map<std::string, std::string> dataStrings;
	std::map<std::string, int> dataInts;
	std::map<std::string, bool> dataBools;
public:
	DataStore();
	~DataStore();

	std::string GetString(std::string key, std::string defaultValue);
	int GetInt(std::string key, int defaultValue);
	bool GetBool(std::string key, bool defaultValue);

	void PutString(std::string key, std::string value);
	void PutInt(std::string key, int value);
	void PutBool(std::string key, bool value);

	std::map<std::string, std::string> GetStringMap();
	std::map<std::string, int> GetIntMap();
	std::map<std::string, bool> GetBoolMap();
};

/*********************************************/

#endif
