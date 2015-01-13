#include "Common/ModelService.h"

static ModelService *s_modelService = nullptr;

ModelService::ModelService() 
{


}
ModelService::~ModelService() 
{
	uninit();
}

// static
ModelService* ModelService::getInstance() 
{
	if (!s_modelService)
	{
		s_modelService = new ModelService();
	}
	s_modelService->retain();
	return s_modelService;
}

void ModelService::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_modelService);
}

void ModelService::init() 
{
	loadConfig();
}

void ModelService::uninit() 
{

}

void ModelService::loadConfig() 
{
	if (!createTableBasicData())
	{
		CCLOG("createTableBasicData failed");
	}

	if (!createTableRoundData())
	{
		CCLOG("createTableRoundData failed");
	}

	initUserData();
}

bool ModelService::createTableBasicData()
{
	std::ostringstream os;
	os << "create table " << TABLE_USER_BASIC_DATA << "(ID integer primary key autoincrement, user_name nvarchar(32), current_round integer)";

	std::string sql = os.str();
	if (!DataBase::getInstance()->createTable(sql))
	{
		return false;
	}
	return true;
}

bool ModelService::createTableRoundData()
{
	std::ostringstream os;
	os << "create table " << TABLE_ROUND_DATA << "(ID integer primary key autoincrement, user_name nvarchar(32), round integer, score integer, best_score integer, best_star integer)";
	std::string sql = os.str();

	if (!DataBase::getInstance()->createTable(sql))
	{
		return false;
	}
	return true;
}

void ModelService::initUserData()
{
	if (DataBase::getInstance()->getDataCount(TABLE_USER_BASIC_DATA) == 0)
	{
		std::ostringstream os;
		os << "insert into " << TABLE_USER_BASIC_DATA << "('user_name', 'current_round')  values('" << USER_NAME "', 0)";
		std::string sql = os.str();
		DataBase::getInstance()->execSQL(sql.c_str());
	}
}

void ModelService::setCurrentRound(int round)
{
	std::string keys[1];
	keys[0] = "current_round";

	std::string values[1];
	values[0] = StringUtils::toString(round);

	DataBaseDataType types[1];
	types[0] = DATA_TYPE_INT;

	DataBase::getInstance()->updateData(TABLE_USER_BASIC_DATA, "user_name", USER_NAME, keys, values, types, 1);
}

void ModelService::setRoundData(int round, int score, int best_score, int star)
{
	std::string keys[5];
	keys[0] = "user_name";
	keys[1] = "round";
	keys[2] = "score";
	keys[3] = "best_score";
	keys[4] = "best_star";

	std::string values[5];
	values[0] = USER_NAME;
	values[1] = StringUtils::toString(round);
	values[2] = StringUtils::toString(score);
	values[3] = StringUtils::toString(best_score);
	values[4] = StringUtils::toString(star);

	DataBaseDataType types[5];
	types[0] = DATA_TYPE_STRING;
	types[1] = DATA_TYPE_INT;
	types[2] = DATA_TYPE_INT;
	types[3] = DATA_TYPE_INT;
	types[4] = DATA_TYPE_INT;

	DataBase::getInstance()->updateData(TABLE_ROUND_DATA, "round", round, keys, values, types, 5);
}

void ModelService::setScore(int round, int score)
{
	std::string keys[3];
	keys[0] = "user_name";
	keys[1] = "round";
	keys[2] = "score";

	std::string values[3];
	values[0] = USER_NAME;
	values[1] = StringUtils::toString(round);
	values[2] = StringUtils::toString(score);

	DataBaseDataType types[3];
	types[0] = DATA_TYPE_STRING;
	types[1] = DATA_TYPE_INT;
	types[2] = DATA_TYPE_INT;

	DataBase::getInstance()->updateData(TABLE_ROUND_DATA, "round", round, keys, values, types, 3);
}

void ModelService::setBestScore(int round, int score)
{
	std::string keys[3];
	keys[0] = "user_name";
	keys[1] = "round";
	keys[2] = "best_score";

	std::string values[3];
	values[0] = USER_NAME;
	values[1] = StringUtils::toString(round);
	values[2] = StringUtils::toString(score);

	DataBaseDataType types[3];
	types[0] = DATA_TYPE_STRING;
	types[1] = DATA_TYPE_INT;
	types[2] = DATA_TYPE_INT;

	DataBase::getInstance()->updateData(TABLE_ROUND_DATA, "round", round, keys, values, types, 3);
}

void ModelService::setBestStar(int round, int star)
{
	std::string keys[3];
	keys[0] = "user_name";
	keys[1] = "round";
	keys[2] = "best_star";

	std::string values[3];
	values[0] = "usertest1";
	values[1] = StringUtils::toString(round);
	values[2] = StringUtils::toString(star);

	DataBaseDataType types[3];
	types[0] = DATA_TYPE_STRING;
	types[1] = DATA_TYPE_INT;
	types[2] = DATA_TYPE_INT;

	DataBase::getInstance()->updateData(TABLE_ROUND_DATA, "round", round, keys, values, types, 3);
}

bool ModelService::getScore(int round, int& score)
{
	return DataBase::getInstance()->getTargetValue(TABLE_ROUND_DATA, "round", round, "score", score);
}
bool ModelService::getBestScore(int round, int& score)
{
	return DataBase::getInstance()->getTargetValue(TABLE_ROUND_DATA, "round", round, "best_score", score);
}
bool ModelService::getBestStar(int round, int& star)
{
	return DataBase::getInstance()->getTargetValue(TABLE_ROUND_DATA, "round", round, "best_star", star);
}

bool ModelService::getCurrentRound(int& round)
{
	return DataBase::getInstance()->getTargetValue(TABLE_USER_BASIC_DATA, "user_name", USER_NAME, "current_round", round);
}