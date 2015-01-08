#ifndef __COMMON_TILED_MAP_PARSER_H__
#define __COMMON_TILED_MAP_PARSER_H__

#include "Common/Resource.h"
#include "Common/Types.h"

namespace cocos2d {
	namespace ui {
		class Button;
	}

}

typedef std::map<uint32_t, Point> MapGidToGamePos;
typedef std::map<std::string, TMXLayerInfo*> MapLayer;
typedef std::map<int, int> MapGidToIndex;

class TiledMapParser : public Ref
{
public:
	static TiledMapParser* create(const std::string& tmxFile);

	TiledMapParser();
	~TiledMapParser();

	// customized interfaces for us.
	TMXLayer* createLayer(const std::string& layerName);
	void getGidToGamePosMap(const std::string& layerName, MapGidToGamePos* map);
	cocos2d::ui::Button* createButton(const Vec2& pos);
	Point gamePosForTileCoord(Point coord);
	Point tileCoordForGamePos(Point position);
	// 解析指定图层的gid矩阵，存入一个以左下角为起点的矩阵中
	void getGidMatrix(const std::string& layerName, int* matrix, int length);
	// 获得所有非空的gid的矩阵下标，从左下角开始
	void getValidGidIndex(const std::string& layerName, std::vector<int>* vecGidIndex);
	void getValidGidToIndexMap(const std::string& layerName, MapGidToIndex* map);
	Size getLayerSize(const std::string& layerName);
	
private:
	bool initWithTMXFile(const std::string& tmxFile);
	TMXLayer* parseLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo);
	TMXTilesetInfo* tilesetForLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo);

	TMXMapInfo* _mapInfo;
	MapLayer _mapLayer;
};


#endif // __COMMON_TILED_MAP_PARSER_H__
