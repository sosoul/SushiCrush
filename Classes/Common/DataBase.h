#ifndef __COMMON_DATA_BASE_H__
#define __COMMON_DATA_BASE_H__

#include "Common/Resource.h"
#include "Common/Types.h"
#include "sqlite3.h"

class DataBase : public Ref
{
public:
	static DataBase *getInstance();
	static void destroyInstance();

	DataBase();
	~DataBase();

	void init();
	void uninit();

	void loadConfig();
	void initUserData();

	bool openDB();
	bool execute(const char *sql);
	char **rawQuery(const char *sql, int *row, int *column, char **result);

	bool createTable(const std::string& sql);

	int getDataCount(const std::string& table_name);
	int getDataCount(const std::string& table_name, const std::string& key, int value);
	int getDataCount(const std::string& table_name, const std::string& key, const std::string& value);

	bool queryValue(const std::string& table_name, const std::string& condition_key, int condition_value, const std::string& target_key, int& target_value);
	bool queryValue(const std::string& table_name, const std::string& condition_key, int condition_value, const std::string& target_key, std::string& target_value);

	bool queryValue(const std::string& table_name, const std::string& condition_key, const std::string& condition_value, const std::string& target_key, int& target_value);
	bool queryValue(const std::string& table_name, const std::string& condition_key, const std::string& condition_value, const std::string& target_key, std::string& target_value);

	int getDataCountBySql(const std::string& sql);

	void updateData(const std::string& table_name, const std::string& key, int value, std::string keys[], std::string values[], DataBaseDataType types[], int key_num);
	void updateData(const std::string& table_name, const std::string& key, const std::string& value, std::string keys[], std::string values[], DataBaseDataType types[], int key_num);

	std::string getValueByType(std::string value, DataBaseDataType type);
};


#endif // __COMMON_DATA_BASE_H__
