#ifndef __COMMON_CONFIG_SERVICE_H__
#define __COMMON_CONFIG_SERVICE_H__

#include "Common/Resource.h"
#include "Common/Types.h"
#include "Common/Utils.h"

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

struct RoundInfo {
	std::string _mapFile;
	std::string _guideMapFile;
	int _clipper[MATRIX_WIDTH*MATRIX_HEIGHT];
	GridType _matrix[MATRIX_WIDTH*MATRIX_HEIGHT];
	VecProducer _vecProducer;
	MapPortal _mapPortalSrcToDest;
	MapPortal _mapPortalDestToSrc;
	MapTarget _mapTarget;
	int _moves;

	RoundInfo() : _moves(0) {
		memset((void*)_clipper, 0, MATRIX_WIDTH*MATRIX_HEIGHT);
		memset((void*)_matrix, GIRD_TYPE_NONE, MATRIX_WIDTH*MATRIX_HEIGHT);
		initTargetMap(&_mapTarget);
	}
};

struct GuideInfo
{
	std::string _mapFile;
	int _matrix[MATRIX_WIDTH*MATRIX_HEIGHT];
	GuideInfo() {
		memset((void*)_matrix, -1, MATRIX_WIDTH*MATRIX_HEIGHT);
	}
};

typedef std::map<int, SushiInfo> MapSushiInfo;
typedef std::map<int, GridInfo> MapGridInfo;
typedef std::map<int, RoundInfo> MapRoundInfo;
typedef std::map<int, GuideInfo> MapGuideInfo;

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
	int getPortalDest(int round, int row, int col);

	int roundCount() { return _roundCount; }
	int guideCount() { return _guideCount; }

	int getImageIndexInGuideMap(int round, int row, int col);

private:
	void parseMap(RoundInfo* roundInfo);
	void parseGuideMap(GuideInfo* guideInfo);
	int _roundCount;
	int _guideCount;
	MapSushiInfo _mapSushiInfo;
	MapGridInfo _mapGridInfo;
	MapRoundInfo _mapRoundInfo;
	MapGuideInfo _mapGuideInfo;
};


#endif // __COMMON_CONFIG_SERVICE_H__
