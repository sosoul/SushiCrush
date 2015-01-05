#ifndef __COMMON_CONFIG_SERVICE_H__
#define __COMMON_CONFIG_SERVICE_H__

#include "Common/Resource.h"
#include "Common/Types.h"

namespace tinyxml2
{
	class XMLElement;
}

struct SushiInfo {
	int _score;

	SushiInfo() : _score(0) {}
};

struct GridInfo {
	int _score;

	GridInfo() : _score(0) {}
};

typedef std::vector<int> VecProducer;
typedef std::map<int, int> MapPortal;
typedef std::map<int, int> MapGidToIndex;

struct RoundInfo {
	std::string _mapFile;
	GridType _layoutInfo[MATRIX_WIDTH*MATRIX_HEIGHT];
	VecProducer _vecProducer;
	MapPortal _mapPortal;
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

	bool isProducer(int round, int row, int col);
	int getPortalSrc(int round, int row, int col);

private:
	void parseMap(RoundInfo* roundInfo);
	void parseLayout(tinyxml2::XMLElement *layerEle, GridType* layout);
	void parseProducer(tinyxml2::XMLElement *layerEle, VecProducer* vecProducer);
	void parsePortal(tinyxml2::XMLElement *layerEle, MapGidToIndex* mapSrcOrDest);
	int _roundCount;
	MapSushiInfo _mapSushiInfo;
	MapGridInfo _mapGridInfo;
	MapRoundInfo _mapRoundInfo;
};


#endif // __COMMON_CONFIG_SERVICE_H__
