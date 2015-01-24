#ifndef __COMMON_MODEL_SERVICE_H__
#define __COMMON_MODEL_SERVICE_H__

#include "Common/DataBase.h"
#include "Common/Resource.h"
#include "Common/Types.h"

class ModelService : public Ref
{
public:
	static ModelService *getInstance();
	static void destroyInstance();

	ModelService();
	~ModelService();

	void init();
	void uninit();

	void loadConfig();

	bool createTableBasicData();
	bool createTableRoundData();

	void initUserData();

	void setRoundData(int round, int score, int best_score, int best_star);

	void setCurrentRound(int round);
	void setMaxPassedRound(int round);

	void setScore(int round, int score);
	void setBestScore(int round, int score);
	void setBestStar(int round, int star);

	bool getScore(int round, int& score);
	bool getBestScore(int round, int& score);
	bool getBestStar(int round, int& star);
	bool getCurrentRound(int& round);
	bool getMaxPassedRound(int& round);
};


#endif // __COMMON_MODEL_SERVICE_H__
