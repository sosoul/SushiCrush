#include "Common/GameDB.h"

static GameDB *s_gameDB = nullptr;

GameDB::GameDB() 
{

}

GameDB::~GameDB() 
{
	uninit();
}

// static
GameDB* GameDB::getInstance() 
{
	if (!s_gameDB)
	{
		s_gameDB = new GameDB();
	}
	s_gameDB->retain();
	return s_gameDB;
}

void GameDB::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_gameDB);
}

void GameDB::init() 
{
	loadConfig();
}

void GameDB::uninit() 
{


}

void GameDB::loadConfig() 
{

	if (!UserDefault::sharedUserDefault()->getBoolForKey(BASE_DB_IS_INITED))
	{
		initUserData();

		UserDefault::sharedUserDefault()->setBoolForKey(BASE_DB_IS_INITED, true);
	}

}

void GameDB::initUserData()
{

}

void getBasicData(const std::string& key, bool& value)
{
	value = UserDefault::sharedUserDefault()->getBoolForKey(key.c_str());
}

void setBasicData(const std::string& key, bool value)
{
	UserDefault::sharedUserDefault()->setBoolForKey(key.c_str(), value);
}

void getBasicData(const std::string& key, int& value)
{
	value = UserDefault::sharedUserDefault()->getIntegerForKey(key.c_str());
}

void setBasicData(const std::string& key, int value)
{
	UserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(), value);
}

void getBasicData(const std::string& key, double& value)
{
	value = UserDefault::sharedUserDefault()->getDoubleForKey(key.c_str());
}

void setBasicData(const std::string& key, double value)
{
	UserDefault::sharedUserDefault()->setDoubleForKey(key.c_str(), value);
}

void getBasicData(const std::string& key, float& value)
{
	value = UserDefault::sharedUserDefault()->getFloatForKey(key.c_str());
}

void setBasicData(const std::string& key, float value)
{
	UserDefault::sharedUserDefault()->setFloatForKey(key.c_str(), value);
}

void getBasicData(const std::string& key, std::string& value)
{
	value = UserDefault::sharedUserDefault()->getStringForKey(key.c_str());
}

void setBasicData(const std::string& key, std::string value)
{
	UserDefault::sharedUserDefault()->setStringForKey(key.c_str(), value);
}