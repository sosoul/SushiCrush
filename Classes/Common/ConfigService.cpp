#include "Common/ConfigService.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "tinyxml2/tinyxml2.h"
#include "Common/TiledMapParser.h"

using namespace tinyxml2;

static ConfigService *s_sharedConfigService = nullptr;

static inline void split(const std::string& src, const std::string& token, std::vector<std::string>& vect)
{
	size_t nend = 0;
	size_t nbegin = 0;
	size_t tokenSize = token.size();
	while (nend != std::string::npos)
	{
		nend = src.find(token, nbegin);
		if (nend == std::string::npos)
			vect.push_back(src.substr(nbegin, src.length() - nbegin));
		else
			vect.push_back(src.substr(nbegin, nend - nbegin));
		nbegin = nend + tokenSize;
	}
}

ConfigService::ConfigService() {
}
ConfigService::~ConfigService() {
	uninit();
}

// static
ConfigService* ConfigService::getInstance() {
	if (!s_sharedConfigService)
	{
		s_sharedConfigService = new ConfigService();
	}
	s_sharedConfigService->retain();
	return s_sharedConfigService;
}

void ConfigService::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_sharedConfigService);
}

void ConfigService::init() {
	_roundCount = 0;
	loadConfig();
}

void ConfigService::uninit() {
	_mapSushiInfo.clear();
	_mapSushiInfo.clear();
	_mapSushiInfo.clear();
}

void ConfigService::loadConfig() {
	std::string filePath = FileUtils::getInstance()->fullPathForFilename(s_commonConfig);
	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
	XMLError errorId = doc->LoadFile(filePath.c_str());

	if (errorId != 0) {
		return;
	}

	XMLElement *configEle = doc->FirstChildElement("config");
	if (!configEle)
		return;

	const XMLAttribute* attribute = NULL;

	// _mapRoundInfo
	if (XMLElement *roundsEle = configEle->FirstChildElement("rounds")) {
		if (attribute = roundsEle->FirstAttribute())
			_roundCount = attribute->IntValue();

		XMLElement* roundEle = roundsEle->FirstChildElement("round");
		while (roundEle) {
			int id = 0;
			RoundInfo item;

			// id
			if (attribute = roundEle->FirstAttribute())
				id = attribute->IntValue();
			// map
			if (XMLElement* mapEle = roundEle->FirstChildElement("map")) {
				item._mapFile = mapEle->GetText();
				parseMap(&item);
			}

			// targetScore
			if (XMLElement* targetEle = roundEle->FirstChildElement("target")) {
				XMLElement* targetItemEle = nullptr;
				if (targetItemEle = targetEle->FirstChildElement("score")) {
					int score;
					targetItemEle->QueryIntText(&score);
					item._mapTarget[TARGET_TYPE_SCORE] = score;
				}
				if (targetItemEle = targetEle->FirstChildElement("jelly")) {
					int jelly;
					targetItemEle->QueryIntText(&jelly);
					item._mapTarget[TARGET_TYPE_JELLY] = jelly;
				}
				if (targetItemEle = targetEle->FirstChildElement("doubleJelly")) {
					int doubleJelly;
					targetItemEle->QueryIntText(&doubleJelly);
					item._mapTarget[TARGET_TYPE_DOUBLE_JELLY] = doubleJelly;
				}
				if (targetItemEle = targetEle->FirstChildElement("cream")) {
					int cream;
					targetItemEle->QueryIntText(&cream);
					item._mapTarget[TARGET_TYPE_CREAM] = cream;
				}
				if (targetItemEle = targetEle->FirstChildElement("doubleCream")) {
					int doubleCream;
					targetItemEle->QueryIntText(&doubleCream);
					item._mapTarget[TARGET_TYPE_DOUBLE_CREAM] = doubleCream;
				}
				if (targetItemEle = targetEle->FirstChildElement("sushi1")) {
					int sushi1;
					targetItemEle->QueryIntText(&sushi1);
					item._mapTarget[TARGET_TYPE_SUSHI_1] = sushi1;
				}
				if (targetItemEle = targetEle->FirstChildElement("sushi2")) {
					int sushi2;
					targetItemEle->QueryIntText(&sushi2);
					item._mapTarget[TARGET_TYPE_SUSHI_2] = sushi2;
				}
				if (targetItemEle = targetEle->FirstChildElement("sushi3")) {
					int sushi3;
					targetItemEle->QueryIntText(&sushi3);
					item._mapTarget[TARGET_TYPE_SUSHI_3] = sushi3;
				}
				if (targetItemEle = targetEle->FirstChildElement("sushi4")) {
					int sushi4;
					targetItemEle->QueryIntText(&sushi4);
					item._mapTarget[TARGET_TYPE_SUSHI_4] = sushi4;
				}
				if (targetItemEle = targetEle->FirstChildElement("sushi5")) {
					int sushi5;
					targetItemEle->QueryIntText(&sushi5);
					item._mapTarget[TARGET_TYPE_SUSHI_5] = sushi5;
				}
				if (targetItemEle = targetEle->FirstChildElement("sushi6")) {
					int sushi6;
					targetItemEle->QueryIntText(&sushi6);
					item._mapTarget[TARGET_TYPE_SUSHI_6] = sushi6;
				}
			}

			// moves
			if (XMLElement* movesEle = roundEle->FirstChildElement("moves")) {
				movesEle->QueryIntText(&item._moves);
			}

			_mapRoundInfo.insert(MapRoundInfo::value_type(id, item));

			roundEle = roundEle->NextSiblingElement();
		}
	}
	

	// _mapSushiInfo
	if (XMLElement* sushisEle = configEle->FirstChildElement("sushis")) {
		XMLElement* sushiEle = sushisEle->FirstChildElement("sushi");
		while (sushiEle) {
			SushiInfo item;
			SushiType type;
			if (attribute = sushiEle->FirstAttribute())
				type = (SushiType)attribute->IntValue();
			if (XMLElement* score = sushiEle->FirstChildElement("score"))
				score->QueryIntText(&item._score);
			_mapSushiInfo.insert(MapSushiInfo::value_type(type, item));
			sushiEle = sushiEle->NextSiblingElement();
		}
	}

	// _mapGridInfo
	if (XMLElement* gridsEle = configEle->FirstChildElement("grids")) {
		XMLElement* gridEle = gridsEle->FirstChildElement("grid");
		while (gridEle) {
			GridInfo item;
			GridType type;
			if (attribute = gridEle->FirstAttribute())
				type = (GridType)attribute->IntValue();
			if (XMLElement* score = gridEle->FirstChildElement("score"))
				score->QueryIntText(&item._score);
			_mapGridInfo.insert(MapGridInfo::value_type(type, item));
			gridEle = gridEle->NextSiblingElement();
		}
	}

	// _mapGuideInfo
	if (XMLElement* guidesEle = configEle->FirstChildElement("guides")) {
		XMLElement* roundEle = guidesEle->FirstChildElement("round");
		if (attribute = guidesEle->FirstAttribute())
			_guideCount = attribute->IntValue();
		while (roundEle) {
			int id = 0;
			GuideInfo item;

			// id
			if (attribute = roundEle->FirstAttribute())
				id = attribute->IntValue();
			// map
			if (XMLElement* mapEle = roundEle->FirstChildElement("map")) {
				item._mapFile = mapEle->GetText();
				parseGuideMap(&item);
			}

			_mapGuideInfo.insert(MapGuideInfo::value_type(id, item));

			roundEle = roundEle->NextSiblingElement();
		}
	}

	delete doc;
}

const SushiInfo* ConfigService::getSushiInfo(SushiType type) const {
	MapSushiInfo::const_iterator it = _mapSushiInfo.find((int)type);
	if (_mapSushiInfo.end() != it)
		return &(it->second);
	return NULL;
	
}

const GridInfo* ConfigService::getGridInfo(GridType type) const {
	MapGridInfo::const_iterator it = _mapGridInfo.find((int)type);
	if (_mapGridInfo.end() != it)
		return &(it->second);
	return NULL;
}

const RoundInfo* ConfigService::getRoundInfo(int round) const {
	MapRoundInfo::const_iterator it = _mapRoundInfo.find(round);
	if (_mapRoundInfo.end() != it)
		return &(it->second);
	return NULL;
}

void ConfigService::parseMap(RoundInfo* roundInfo) {
	if (!roundInfo)
		return;

	std::string filePath = FileUtils::getInstance()->fullPathForFilename(roundInfo->_mapFile);
	TiledMapParser* tiledMapParser = TiledMapParser::create(filePath.c_str());
	if (!tiledMapParser)
		return;

	int clipper[MATRIX_WIDTH*MATRIX_HEIGHT];
	Size size = tiledMapParser->getLayerSize("clipperLayer");
	if (MATRIX_WIDTH == size.width && MATRIX_HEIGHT == size.height) {
		tiledMapParser->getGidMatrix("clipperLayer", clipper, MATRIX_WIDTH*MATRIX_HEIGHT);
		for (int i = 0; i < MATRIX_WIDTH*MATRIX_HEIGHT; ++i) {
			roundInfo->_clipper[i] = clipper[i];
		}
	}

	int grid[MATRIX_WIDTH*MATRIX_HEIGHT];
	size = tiledMapParser->getLayerSize("gridLayer");
	if (MATRIX_WIDTH == size.width && MATRIX_HEIGHT == size.height) {
		tiledMapParser->getGidMatrix("gridLayer", grid, MATRIX_WIDTH*MATRIX_HEIGHT);
		for (int i = 0; i < MATRIX_WIDTH*MATRIX_HEIGHT; ++i) {
			GridType type = GRID_TYPE_NONE;
			switch (grid[i])
			{
			case 1:
				type = GRID_TYPE_CREAM;
				break;
			case 2:
				type = GRID_TYPE_DOUBLE_CREAM;
				break;
			case 3:
				type = GRID_TYPE_DOUBLE_JELLY;
				break;
			case 4:
				type = GRID_TYPE_JELLY;
				break;
			case 5:
				type = GRID_TYPE_NORMAL;
				break;
			default:
				break;
			}
			roundInfo->_grid[i] = type;
		}
	}

	tiledMapParser->getValidGidIndex("producerLayer", &roundInfo->_vecProducer);
	MapGidToIndex mapPortalSrc;
	MapGidToIndex mapPortalDest;
	tiledMapParser->getValidGidToIndexMap("portalSrcLayer", &mapPortalSrc);
	tiledMapParser->getValidGidToIndexMap("portalDestLayer", &mapPortalDest);
	std::map<int, int>::iterator itDest = mapPortalDest.begin();
	for (; mapPortalDest.end() != itDest; ++itDest)
	{
		std::map<int, int>::iterator itSrc = mapPortalSrc.find(itDest->first);
		if (mapPortalSrc.end() != itSrc) {
			roundInfo->_mapPortalDestToSrc.insert(MapPortal::value_type(itDest->second, itSrc->second));
			roundInfo->_mapPortalSrcToDest.insert(MapPortal::value_type(itSrc->second, itDest->second));
		}
	}
}

void ConfigService::parseGuideMap(GuideInfo* guideInfo) {
	if (!guideInfo)
		return;

	std::string filePath = FileUtils::getInstance()->fullPathForFilename(guideInfo->_mapFile);
	TiledMapParser* tiledMapParser = TiledMapParser::create(filePath.c_str());
	if (!tiledMapParser)
		return;

	int sushi[MATRIX_WIDTH*MATRIX_HEIGHT];
	Size size = tiledMapParser->getLayerSize("sushiLayer");
	if (MATRIX_WIDTH == size.width && MATRIX_HEIGHT == size.height) {
		tiledMapParser->getGidMatrix("sushiLayer", sushi, MATRIX_WIDTH*MATRIX_HEIGHT);
		for (int i = 0; i < MATRIX_WIDTH*MATRIX_HEIGHT; ++i) {
			int index = -1;
			switch (sushi[i])
			{
			case 8:
				index = 0;
				break;
			case 12:
				index = 1;
				break;
			case 16:
				index = 2;
				break;
			case 20:
				index = 3;
				break;
			case 25:
				index = 4;
				break;
			case 29:
				index = 5;
			default:
				break;
			}
			guideInfo->_sushi[i] = index;
		}
	}

	int grid[MATRIX_WIDTH*MATRIX_HEIGHT];
	size = tiledMapParser->getLayerSize("gridLayer");
	if (MATRIX_WIDTH == size.width && MATRIX_HEIGHT == size.height) {
		tiledMapParser->getGidMatrix("gridLayer", grid, MATRIX_WIDTH*MATRIX_HEIGHT);
		for (int i = 0; i < MATRIX_WIDTH*MATRIX_HEIGHT; ++i) {
			GridType type = GRID_TYPE_NONE;
			switch (grid[i])
			{
			case 1:
				type = GRID_TYPE_CREAM;
				break;
			case 2:
				type = GRID_TYPE_DOUBLE_CREAM;
				break;
			case 3:
				type = GRID_TYPE_DOUBLE_JELLY;
				break;
			case 4:
				type = GRID_TYPE_JELLY;
				break;
			case 5:
				type = GRID_TYPE_NORMAL;
				break;
			default:
				break;
			}
			guideInfo->_grid[i] = type;
		}
	}
}

bool ConfigService::isProducer(int round, int row, int col) {
	MapRoundInfo::iterator it = _mapRoundInfo.find(round);
	if (_mapRoundInfo.end() == it)
		return false;
	VecProducer::iterator itVec = std::find(it->second._vecProducer.begin(),
		it->second._vecProducer.end(), MATRIX_WIDTH*row + col);
	if (it->second._vecProducer.end() == itVec)
		return false;
	return true;
}

int ConfigService::getPortalSrc(int round, int row, int col) {
	MapRoundInfo::iterator it = _mapRoundInfo.find(round);
	if (_mapRoundInfo.end() == it)
		return -1;
	MapPortal::iterator itMap = it->second._mapPortalDestToSrc.find(MATRIX_WIDTH*row + col);
	if (it->second._mapPortalDestToSrc.end() == itMap)
		return -1;
	return itMap->second;
}

int ConfigService::getPortalDest(int round, int row, int col) {
	std::pair<int, int> pair;
	MapRoundInfo::iterator it = _mapRoundInfo.find(round);
	if (_mapRoundInfo.end() == it)
		return -1;
	MapPortal::iterator itMap = it->second._mapPortalSrcToDest.find(MATRIX_WIDTH*row + col);
	if (it->second._mapPortalSrcToDest.end() == itMap)
		return -1;
	return itMap->second;
}

int ConfigService::getImageIndexInGuideMap(int round, int row, int col) {
	CCASSERT(row >= 0 && row < MATRIX_WIDTH && col >= 0 && col < MATRIX_HEIGHT, "row or col is out of range!");
	MapGuideInfo::iterator it = _mapGuideInfo.find(round);
	if (_mapGuideInfo.end() == it)
		return -1;
	return it->second._sushi[MATRIX_WIDTH*row + col];
}

GridType ConfigService::getGridTypeInGuideMap(int round, int row, int col) {
	CCASSERT(row >= 0 && row < MATRIX_WIDTH && col >= 0 && col < MATRIX_HEIGHT, "row or col is out of range!");
	MapGuideInfo::iterator it = _mapGuideInfo.find(round);
	if (_mapGuideInfo.end() == it)
		return GRID_TYPE_NONE;
	return it->second._grid[MATRIX_WIDTH*row + col];
}