#ifndef __COMMON_CONFIG_SERVICE_H__
#define __COMMON_CONFIG_SERVICE_H__

#include "Common/Resource.h"
#include "Common/Types.h"

struct SushiInfo {
	int _score;

	SushiInfo() : _score(0) {}
};

struct GridInfo {
	int _score;

	GridInfo() : _score(0) {}
};

struct RoundInfo {
	std::string _mapFile;
	GridType _layoutInfo[MATRIX_WIDTH*MATRIX_HEIGHT];
	int _targetScore;
	int _moves;

	RoundInfo() : _targetScore(0), _moves(0) {
		memset((void*)_layoutInfo, GIRD_TYPE_NONE, MATRIX_WIDTH*MATRIX_HEIGHT);
	}
};


typedef std::map<int, SushiInfo> MapSushiInfo;
typedef std::map<int, GridInfo> MapGridInfo;
typedef std::map<int, RoundInfo> MapRoundInfo;


class ConfigService : public Ref
{
public:
	static ConfigService *getInstance();
	static void destroyInstance();

	ConfigService();
	~ConfigService();

	void init();
	void uninit();

	void loadConfig();

	const SushiInfo* getSushiInfo(SushiType type) const;
	const GridInfo* getGridInfo(GridType type) const;
	const RoundInfo* getRoundInfo(int round) const;

private:
	void parseMap(std::string mapFile, GridType* layout);
	int _roundCount;
	MapSushiInfo _mapSushiInfo;
	MapGridInfo _mapGridInfo;
	MapRoundInfo _mapRoundInfo;
};


#endif // __COMMON_CONFIG_SERVICE_H__
