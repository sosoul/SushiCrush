#ifndef __COMMON_GAME_DB_H__
#define __COMMON_GAME_DB_H__

#include "Common/Resource.h"
#include "Common/Types.h"

class GameDB : public Ref
{
public:
	static GameDB *getInstance();
	static void destroyInstance();

	GameDB();
	~GameDB();

	void init();
	void uninit();

	void loadConfig();
	void initUserData();

	void getBasicData(const std::string& key, bool& value);
	void setBasicData(const std::string& key, bool value);

	void getBasicData(const std::string& key, int& value);
	void setBasicData(const std::string& key, int value);

	void getBasicData(const std::string& key, double& value);
	void setBasicData(const std::string& key, double value);

	void getBasicData(const std::string& key, float& value);
	void setBasicData(const std::string& key, float value);

	void getBasicData(const std::string& key, std::string& value);
	void setBasicData(const std::string& key, std::string value);

};


#endif // __COMMON_GAME_DB_H__
