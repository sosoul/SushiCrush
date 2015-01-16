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

	int matrix[MATRIX_WIDTH*MATRIX_HEIGHT];
	size = tiledMapParser->getLayerSize("matrixLayer");
	if (MATRIX_WIDTH == size.width && MATRIX_HEIGHT == size.height) {
		tiledMapParser->getGidMatrix("matrixLayer", matrix, MATRIX_WIDTH*MATRIX_HEIGHT);
		for (int i = 0; i < MATRIX_WIDTH*MATRIX_HEIGHT; ++i) {
			GridType type = GIRD_TYPE_NONE;
			switch (matrix[i])
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
				type = GIRD_TYPE_NORMAL;
				break;
			default:
				break;
			}
			roundInfo->_matrix[i] = type;
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
		return false;
	MapPortal::iterator itMap = it->second._mapPortalDestToSrc.find(MATRIX_WIDTH*row + col);
	if (it->second._mapPortalDestToSrc.end() == itMap)
		return -1;
	return itMap->second;
}

int ConfigService::getPortalDest(int round, int row, int col) {
	std::pair<int, int> pair;
	MapRoundInfo::iterator it = _mapRoundInfo.find(round);
	if (_mapRoundInfo.end() == it)
		return false;
	MapPortal::iterator itMap = it->second._mapPortalSrcToDest.find(MATRIX_WIDTH*row + col);
	if (it->second._mapPortalSrcToDest.end() == itMap)
		return -1;
	return itMap->second;
}