#include "Common/TiledMapParser.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

TiledMapParser::TiledMapParser() {
}

TiledMapParser::~TiledMapParser() {
}

TiledMapParser* TiledMapParser::create(const std::string& tmxFile) {
	TiledMapParser *ret = new (std::nothrow) TiledMapParser();
	if (ret->initWithTMXFile(tmxFile))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool TiledMapParser::initWithTMXFile(const std::string& tmxFile) {
	_mapInfo = TMXMapInfo::create(tmxFile);
	if (!_mapInfo)
	{
		return false;
	}
	CCASSERT(!_mapInfo->getTilesets().empty(), "TMXTiledMap: Map not found. Please check the filename.");

	TMXLayerInfo* layerInfo = NULL;
	auto& layers = _mapInfo->getLayers();
	for (const auto &item : layers) {
		if (item)
			_mapLayer.insert(MapLayer::value_type(item->_name, item));
	}
	return true;
}

TMXLayer* TiledMapParser::createLayer(const std::string& layerName) {
	auto& layers = _mapInfo->getLayers();
	for (const auto &layerInfo : layers) {
		if (layerInfo->_name == layerName) {
			return parseLayer(layerInfo, _mapInfo);
		}
	}
	return nullptr;
}

TMXLayer* TiledMapParser::parseLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo) {
	TMXTilesetInfo *tileset = tilesetForLayer(layerInfo, mapInfo);
	TMXLayer *layer = TMXLayer::create(tileset, layerInfo, mapInfo);

	// tell the layerinfo to release the ownership of the tiles map.
	layerInfo->_ownTiles = false;
	layer->setupTiles();

	return layer;
}

TMXTilesetInfo* TiledMapParser::tilesetForLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo) {
	Size size = layerInfo->_layerSize;
	auto& tilesets = mapInfo->getTilesets();
	if (tilesets.size() > 0)
	{
		TMXTilesetInfo* tileset = nullptr;
		for (auto iter = tilesets.crbegin(); iter != tilesets.crend(); ++iter)
		{
			tileset = *iter;
			if (tileset)
			{
				for (int y = 0; y < size.height; y++)
				{
					for (int x = 0; x < size.width; x++)
					{
						int pos = static_cast<int>(x + size.width * y);
						int gid = layerInfo->_tiles[pos];

						// gid are stored in little endian.
						// if host is big endian, then swap
						//if( o == CFByteOrderBigEndian )
						//    gid = CFSwapInt32( gid );
						/* We support little endian.*/

						// FIXME:: gid == 0 --> empty tile
						if (gid != 0)
						{
							// Optimization: quick return
							// if the layer is invalid (more than 1 tileset per layer) an CCAssert will be thrown later
							if ((gid & kTMXFlippedMask) >= tileset->_firstGid)
								return tileset;
						}
					}
				}
			}
		}
	}

	// If all the tiles are 0, return empty tileset
	CCLOG("cocos2d: Warning: TMX Layer '%s' has no tiles", layerInfo->_name.c_str());
	return nullptr;
}

Point TiledMapParser::gamePosForTileCoord(Point coord) {
	if (!_mapInfo)
		return Point();
	int x = coord.x * _mapInfo->getTileSize().width + _mapInfo->getTileSize().width / 2;
	int y = _mapInfo->getMapSize().height * _mapInfo->getTileSize().height - (coord.y * _mapInfo->getTileSize().height + _mapInfo->getTileSize().height / 2);
	return Point(x, y);
}

Point TiledMapParser::tileCoordForGamePos(Point position) {
	if (!_mapInfo)
		return Point();
	int x = position.x / _mapInfo->getTileSize().width;
	int y = ((_mapInfo->getMapSize().height * _mapInfo->getTileSize().height) - position.y) / _mapInfo->getTileSize().height;
	return Point(x, y);
}

cocos2d::ui::Button* TiledMapParser::createButton(const Vec2& pos) {
	return nullptr;
}

void TiledMapParser::getGidToGamePosMap(const std::string& layerName, MapGidToGamePos* map) {
	if (!map)
		return;

	TMXLayerInfo* layerInfo = NULL;
	MapLayer::iterator it = _mapLayer.find(layerName);
	if (_mapLayer.end() != it)
		layerInfo = it->second;

	if (!layerInfo)
		return;

	Size size = layerInfo->_layerSize;
	for (int row = 0; row < size.height; ++row) {
		for (int col = 0; col < size.width; ++col) {
			ssize_t idx = static_cast<int>((col + row * layerInfo->_layerSize.width));
			uint32_t tile = layerInfo->_tiles[idx];
			uint32_t gid = (tile & kTMXFlippedMask);
			if (0 != gid) {
				Point pos = gamePosForTileCoord(Point(col, row));
				map->insert(MapGidToGamePos::value_type(gid, pos));
			}
		}
	}

	return;
}

void TiledMapParser::getGidMatrix(const std::string& layerName, int* matrix, int length) {
	if (!matrix || length <= 0)
		return;

	TMXLayerInfo* layerInfo = NULL;
	MapLayer::iterator it = _mapLayer.find(layerName);
	if (_mapLayer.end() != it)
		layerInfo = it->second;

	if (!layerInfo)
		return;

	Size size = layerInfo->_layerSize;
	int index = 0;
	for (int row = size.height - 1; row >= 0; --row) {
		for (int col = 0; col < size.width; ++col) {
			ssize_t idx = static_cast<int>((col + row * layerInfo->_layerSize.width));
			uint32_t tile = layerInfo->_tiles[idx];
			uint32_t gid = (tile & kTMXFlippedMask);
			if (index >= length)
				return;
			matrix[index++] = gid;
		}
	}
}

void TiledMapParser::getValidGidIndex(const std::string& layerName, std::vector<int>* vecGidIndex) {
	if (!vecGidIndex)
		return;

	TMXLayerInfo* layerInfo = NULL;
	MapLayer::iterator it = _mapLayer.find(layerName);
	if (_mapLayer.end() != it)
		layerInfo = it->second;

	if (!layerInfo)
		return;

	Size size = layerInfo->_layerSize;
	int index = 0;
	for (int row = size.height - 1; row >= 0; --row) {
		for (int col = 0; col < size.width; ++col) {
			ssize_t idx = static_cast<int>((col + row * layerInfo->_layerSize.width));
			uint32_t tile = layerInfo->_tiles[idx];
			uint32_t gid = (tile & kTMXFlippedMask);
			if (0 != gid)
				vecGidIndex->push_back(index);
			++index;
		}
	}
}

void TiledMapParser::getValidGidToIndexMap(const std::string& layerName, MapGidToIndex* map) {
	if (!map)
		return;

	TMXLayerInfo* layerInfo = NULL;
	MapLayer::iterator it = _mapLayer.find(layerName);
	if (_mapLayer.end() != it)
		layerInfo = it->second;

	if (!layerInfo)
		return;

	Size size = layerInfo->_layerSize;
	int index = 0;
	for (int row = size.height - 1; row >= 0; --row) {
		for (int col = 0; col < size.width; ++col) {
			ssize_t idx = static_cast<int>((col + row * layerInfo->_layerSize.width));
			uint32_t tile = layerInfo->_tiles[idx];
			uint32_t gid = (tile & kTMXFlippedMask);
			if (0 != gid)
				map->insert(MapGidToIndex::value_type(gid, index));
			++index;
		}
	}
}

Size TiledMapParser::getLayerSize(const std::string& layerName) {
	MapLayer::iterator it = _mapLayer.find(layerName);
	if (_mapLayer.end() != it && it->second)
		return it->second->_layerSize;
	return Size();
}