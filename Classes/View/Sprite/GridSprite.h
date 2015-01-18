#ifndef __SPRITE_GRID_SPRITE_H__
#define __SPRITE_GRID_SPRITE_H__

#include "cocos2d.h"
#include "Common/Types.h"

USING_NS_CC;

class GridSprite : public Sprite
{
public:
	GridSprite();
	~GridSprite();
	static GridSprite *create(int row, int col, GridType type);

	CC_SYNTHESIZE(int, _row, Row);
	CC_SYNTHESIZE(int, _col, Col);
	CC_SYNTHESIZE_READONLY(GridType, _gridType, GridType);
	CC_SYNTHESIZE_READONLY(GridType, _originGridType, OriginGridType);
	void setGridType(GridType type);

private:
	
};

#endif // __SPRITE_GRID_SPRITE_H__
