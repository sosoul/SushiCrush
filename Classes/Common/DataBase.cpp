#include "Common/DataBase.h"

static DataBase *s_database = nullptr;
static sqlite3* s_pointer_db = NULL; //数据库指针
DataBase::DataBase()
{

}

DataBase::~DataBase()
{
	uninit();
}

// static
DataBase* DataBase::getInstance()
{
	if (!s_database)
	{
		s_database = new DataBase();
	}
	s_database->retain();
	return s_database;
}

void DataBase::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_database);
}

void DataBase::init()
{
	loadConfig();
}

void DataBase::uninit()
{


}

void DataBase::loadConfig()
{

}


bool DataBase::openDB()
{
	std::string sqlStr; //sql指令
	int result; //sql_exec返回指令

	std::string fileName = CCFileUtils::getInstance()->fullPathForFilename("sushiData.db");

	CCLOG("db path: %s", fileName.c_str());

	result = sqlite3_open(fileName.c_str(), &s_pointer_db);
	if (result != SQLITE_OK)
	{
		CCLOG("fail open errCode:%d\n", result);
		return false;
	}
	return true;
}

bool DataBase::createTable(const std::string& sql)
{
	char *errMsg; //错误信息
	std::string sqlStr; //sql指令
	int result; //sql_exec返回指令

	if (s_pointer_db == NULL)
	{
		if (!openDB())
		{
			CCLOG("createUserBasicData fail. Fail open database");
			return false;
		}
	}

	result = sqlite3_exec(s_pointer_db, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
	{
		CCLOG("fail exec code:%d errMsg:%s", result, errMsg);
	}
	sqlite3_close(s_pointer_db);
	s_pointer_db = NULL;
	return true;
}



bool DataBase::execute(const char *sql)
{
	if (s_pointer_db == NULL)
	{
		if (!openDB())
		{
			CCLOG("createUserBasicData fail. Fail open database");
			return false;
		}
	}

	sqlite3_exec(s_pointer_db, sql, 0, 0, 0);
	sqlite3_close(s_pointer_db);
	s_pointer_db = NULL;
	return true;
}
char **DataBase::rawQuery(const char *sql, int *row, int *column, char **result)
{
	if (s_pointer_db == NULL)
	{
		if (!openDB())
		{
			CCLOG("createUserBasicData fail. Fail open database");
			return result;
		}
	}

	sqlite3_get_table(s_pointer_db, sql, &result, row, column, 0);
	sqlite3_close(s_pointer_db);
	s_pointer_db = NULL;
	return result;
}

bool DataBase::queryValue(const std::string& table_name, const std::string& key, int value, const std::string& target_key, int& target_value)
{
	std::ostringstream os;
	os << "select " << target_key << " from " << table_name << " where " << key << " = " << value;
	std::string sql = os.str();
	int row, col;
	char *data = "";
	char **result = &data;
	char **re = rawQuery(sql.c_str(), &row, &col, result);

	if (row > 0 && re != NULL && re[1] != NULL)
	{
		target_value = atoi(re[1]); //(int)re[1];
		return true;
	}

	return false;
}

bool DataBase::queryValue(const std::string& table_name, const std::string& key, int value, const std::string& target_key, std::string& target_value)
{
	std::ostringstream os;
	os << "select " << target_key << " from " << table_name << " where " << key << " = " << value;
	std::string sql = os.str();
	int row, col;
	char *data = "";
	char **result = &data;
	char **re = rawQuery(sql.c_str(), &row, &col, result);

	if (re != NULL)
	{
		target_value = re[1];
	}

	return true;
}

bool DataBase::queryValue(const std::string& table_name, const std::string& key, const std::string& value, const std::string& target_key, int& target_value)
{
	std::ostringstream os;
	os << "select " << target_key << " from " << table_name << " where " << key << " = '" << value << "'";
	std::string sql = os.str();
	int row, col;
	char *data = "";
	char **result = &data;
	char **re = rawQuery(sql.c_str(), &row, &col, result);

	if (re != NULL)
	{
		target_value = atoi(re[1]); //(int)re[1];
		return true;
	}

	return false;
}

bool DataBase::queryValue(const std::string& table_name, const std::string& key, const std::string& value, const std::string& target_key, std::string& target_value)
{
	std::ostringstream os;
	os << "select " << target_key << " from " << table_name << " where " << key << " = '" << value << "'";
	std::string sql = os.str();
	int row, col;
	char *data = "";
	char **result = &data;
	char **re = rawQuery(sql.c_str(), &row, &col, result);

	if (re != NULL)
	{
		target_value = re[1]; //(int)re[1];
		return true;
	}

	return false;
}


int DataBase::getDataCount(const std::string& table_name)
{
	std::string sql = "select * from ";
	sql += table_name;

	return getDataCountBySql(sql);
}

int DataBase::getDataCount(const std::string& table_name, const std::string& key, int value)
{
	std::ostringstream os;
	os << "select * from " << table_name << " where " << key << " = " << value;
	std::string sql = os.str();

	return getDataCountBySql(sql);
}

int DataBase::getDataCount(const std::string& table_name, const std::string& key, const std::string& value)
{
	std::ostringstream os;
	os << "select * from " << table_name << " where " << key << " = '" << value << "'";
	std::string sql = os.str();

	return getDataCountBySql(sql);
}

int DataBase::getDataCountBySql(const std::string& sql)
{
	int row, col;
	char *data = "";
	char **result = &data;
	char **re = rawQuery(sql.c_str(), &row, &col, result);

	return row;
}

std::string DataBase::getValueByType(std::string value, DataBaseDataType type)
{
	if (type == DATA_TYPE_STRING)
	{
		return "'" + value + "'";
	}
	else
	{
		return value;
	}
}

void DataBase::updateData(const std::string& table_name, const std::string& key, int value, std::string keys[], std::string values[], DataBaseDataType types[],int key_num)
{
	if (getDataCount(table_name, key, value) == 0)
	{
		//insert
		std::ostringstream os;
		os << "insert into " << table_name << "(";
		os << "" << keys[0] << "";

		for (int i = 1; i < key_num; i++)
		{
			os << ", " << keys[i] << "";
		}
		
		os << ") values(";
		os << getValueByType(values[0], types[0]);

		for (int j = 1; j < key_num; j++)
		{
			os << ", " << getValueByType(values[j], types[j]);
		}

		os << ")";
		
		std::string sql = os.str();

		CCLOG("updateData sql %s", sql.c_str());
		execute(sql.c_str());
	}
	else
	{
		//update
		std::ostringstream os;
		os << "update " << table_name << " set ";
		os << "" << keys[0] << " = " << getValueByType(values[0], types[0]);

		for (int i = 1; i < key_num; i++)
		{
			os << " , " << keys[i] << " = " << getValueByType(values[i], types[i]);
		}

		os << " where " << key << " = " << value;
		std::string sql = os.str();

		CCLOG("updateData sql %s", sql.c_str());
		execute(sql.c_str());
	}
}

void DataBase::updateData(const std::string& table_name, const std::string& key, const std::string& value, std::string keys[], std::string values[], DataBaseDataType types[], int key_num)
{
	if (getDataCount(table_name, key, value) == 0)
	{
		//insert
		std::ostringstream os;
		os << "insert into " << table_name << "(";
		os << "" << keys[0] << "";

		for (int i = 1; i < key_num; i++)
		{
			os << ", " << keys[i] << "";
		}

		os << ") values(";
		os << getValueByType(values[0], types[0]);

		for (int j = 1; j < key_num; j++)
		{
			os << ", " << getValueByType(values[j], types[j]);
		}

		os << ")";

		std::string sql = os.str();

		CCLOG("updateData sql %s", sql.c_str());
		execute(sql.c_str());
	}
	else
	{
		//update
		std::ostringstream os;
		os << "update " << table_name << " set ";
		os << "" << keys[0] << " = " << getValueByType(values[0], types[0]);

		for (int i = 1; i < key_num; i++)
		{
			os << " , " << keys[i] << " = " << getValueByType(values[i], types[i]);
		}

		os << " where " << key << " = '" << value << "'";
		std::string sql = os.str();

		CCLOG("updateData sql %s", sql.c_str());
		execute(sql.c_str());
	}
}