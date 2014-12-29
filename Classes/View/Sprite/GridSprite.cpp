#include "View/Sprite/GridSprite.h"

#include<stdlib.h>
#include "Common/Resource.h"

USING_NS_CC;

namespace {
	const int kNormalZOrder = -20;
	const int kJellyZOrder = 0;
	const int kDoubleJellyZOrder = 0;
	const int kCreamZOrder = 0;
	const int kDoubleCreamZOrder = 0;
}

GridSprite::GridSprite() : m_col(0),
m_row(0)
{
}

GridSprite::~GridSprite() {
}

GridSprite *GridSprite::create(int row, int col, GridType type)
{
	GridSprite *grid = new GridSprite();
	grid->m_row = row;
	grid->m_col = col;
	grid->m_gridType = type;
	switch (type)
	{
	case GIRD_TYPE_NORMAL:
		grid->initWithSpriteFrameName(s_gridNormal);
		grid->setZOrder(kNormalZOrder);
		break;
	case GRID_TYPE_JELLY:
		grid->initWithSpriteFrameName(s_gridJelly);
		grid->setZOrder(kJellyZOrder);
		break;
	case GRID_TYPE_DOUBLE_JELLY:
		grid->initWithSpriteFrameName(s_gridDoubleJelly);
		grid->setZOrder(kDoubleJellyZOrder);
		break;
	case GRID_TYPE_CREAM:
		grid->initWithSpriteFrameName(s_gridCream);
		grid->setZOrder(kCreamZOrder);
		break;
	case GRID_TYPE_DOUBLE_CREAM:
		grid->initWithSpriteFrameName(s_gridDoubleCream);
		grid->setZOrder(kDoubleCreamZOrder);
		break;
	default:
		break;
	}
	grid->autorelease();
	return grid;
}

void GridSprite::setGridType(GridType type)
{
	m_gridType = type;
	SpriteFrame *frame;
	switch (m_gridType)
	{
	case GIRD_TYPE_NORMAL:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s_gridNormal);
		setZOrder(kNormalZOrder);
		break;
	case GRID_TYPE_JELLY:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s_gridJelly);
		setZOrder(kJellyZOrder);
		break;
	case GRID_TYPE_DOUBLE_JELLY:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s_gridDoubleJelly);
		setZOrder(kDoubleJellyZOrder);
		break;
	case GRID_TYPE_CREAM:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s_gridCream);
		setZOrder(kCreamZOrder);
		break;
	case GRID_TYPE_DOUBLE_CREAM:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s_gridDoubleCream);
		setZOrder(kDoubleCreamZOrder);
		break;
	default:
		return;
	}
	setSpriteFrame(frame);
}