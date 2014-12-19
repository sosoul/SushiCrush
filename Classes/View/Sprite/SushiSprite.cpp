#include "View/Sprite/SushiSprite.h"

#include<stdlib.h>
#include "Common/Resource.h"

USING_NS_CC;
//#define DEBUG

const char *sushiNormal[SUSHI_COUNT] = {
	"sushi_1n.png",
	"sushi_2n.png",
	"sushi_3n.png",
	"sushi_4n.png",
	"sushi_5n.png",
	"sushi_6n.png"
};

const char *sushiVertical[SUSHI_COUNT] = {
	"sushi_1v.png",
	"sushi_2v.png",
	"sushi_3v.png",
	"sushi_4v.png",
	"sushi_5v.png",
	"sushi_6v.png"
};

const char *sushiHorizontal[SUSHI_COUNT] = {
	"sushi_1h.png",
	"sushi_2h.png",
	"sushi_3h.png",
	"sushi_4h.png",
	"sushi_5h.png",
	"sushi_6h.png"
};

const char *sushi5Cross[SUSHI_COUNT] = {
	"sushi_1c.png",
	"sushi_2c.png",
	"sushi_3c.png",
	"sushi_4c.png",
	"sushi_5c.png",
	"sushi_6c.png"
};

const char sushi5Line[] = "sushi_5line.png";

const int kTestMatrixes[8][7] = {
		{ 0, 3, 1, 5, 0, 0, 1 },
		{ 1, 1, 0, 1, 3, 4, 5 },
		{ 0, 4, 1, 2, 0, 4, 0 },
		{ 0, 0, 1, 4, 5, 0, 0 },
		{ 2, 3, 4, 5, 0, 0, 1 },
		{ 3, 4, 5, 0, 0, 1, 2 },
		{ 4, 5, 0, 0, 1, 2, 3 },
		{ 5, 0, 0, 1, 2, 3, 4 } };

SushiSprite::SushiSprite() : m_col(0),
							 m_row(0),
							 m_imgIndex(0),
							 m_isNeedRemove(false),
							 m_ignoreCheck(false),
							 m_displayMode(DISPLAY_MODE_NORMAL),
							 m_score(20),
							 m_sushiPriorityLevel(PRIORITY_NORMAL)
{
}

SushiSprite::~SushiSprite() {
}

SushiSprite *SushiSprite::create(int row, int col, int topImgIndex, int leftImgIndex, PriorityLevel priorityLevel)
{
	SushiSprite *sushi = new SushiSprite();
	sushi->m_row = row;
	sushi->m_col = col;
	int index = 0;
#if defined(DEBUG)
	index = kTestMatrixes[row][col];
#else
	do 
	{
		index = rand() % SUSHI_COUNT;
	} while (index == topImgIndex || index == leftImgIndex);
#endif
	sushi->m_sushiPriorityLevel = priorityLevel;
	sushi->m_imgIndex = index;
	sushi->initWithSpriteFrameName(sushiNormal[sushi->m_imgIndex]);
	sushi->autorelease();
	return sushi;
}

float SushiSprite::getContentWidth()
{
	static float itemWidth = 0;
	if (0 == itemWidth) {
		Sprite *sprite = CCSprite::createWithSpriteFrameName(sushiNormal[0]);
		itemWidth = sprite->getContentSize().width;
	}
	return itemWidth;
}

void SushiSprite::setDisplayMode(DisplayMode mode)
{
	m_displayMode = mode;
	switch (m_displayMode)
	{
	case DISPLAY_MODE_NORMAL:
		m_score = 20;
		break;
	case DISPLAY_MODE_4_HORIZONTAL_LINE:
	case DISPLAY_MODE_4_VERTICAL_LINE:
		m_score = 30;
		break;
	case  DISPLAY_MODE_5_LINE:
		m_score = 40;
		break;
	case DISPLAY_MODE_5_CROSS:
		m_score = 50;
		break;
	default:
		break;
	}
	if (mode == DISPLAY_MODE_5_LINE)
	{
		m_imgIndex = 6;
	}
}

void SushiSprite::applyDisplayMode() {
	SpriteFrame *frame;
	switch (m_displayMode) {
	case DISPLAY_MODE_4_VERTICAL_LINE:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushiVertical[m_imgIndex]);
		break;
	case DISPLAY_MODE_4_HORIZONTAL_LINE:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushiHorizontal[m_imgIndex]);
		break;
	case DISPLAY_MODE_5_LINE:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushi5Line);
		break;
	case DISPLAY_MODE_5_CROSS:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushi5Cross[m_imgIndex]);
		break;
	default:
		return;
	}
	setSpriteFrame(frame);
}