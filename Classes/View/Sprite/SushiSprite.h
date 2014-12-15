#ifndef __SushiSprite_SCENE_H__
#define __SushiSprite_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
	DISPLAY_MODE_NORMAL = 0,
	DISPLAY_MODE_4_HORIZONTAL_LINE,
	DISPLAY_MODE_4_VERTICAL_LINE,
	DISPLAY_MODE_5_LINE,
	DISPLAY_MODE_5_CROSS,
	DISPLAY_MODE_7_T_SHAPED,
} DisplayMode;

typedef enum {
	PRIORITY_NORMAL = 0,
	PRIORITY_4_LINE = 1,
	PRIORITY_5_CROSS = 2,
	PRIORITY_5_LINE = 3,
} PriorityLevel;

class SushiSprite : public Sprite
{
public:
	SushiSprite();
	~SushiSprite();
	static SushiSprite *create(int row, int col, int topImgIndex, int leftImgIndex, PriorityLevel priorityLevel);
	static float getContentWidth();

	CC_SYNTHESIZE(int, m_row, Row);
	CC_SYNTHESIZE(int, m_col, Col);
	CC_SYNTHESIZE(int, m_imgIndex, ImgIndex);
	CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);
	CC_SYNTHESIZE(bool, m_ignoreCheck, IgnoreCheck);//新产生的4消寿司，本轮不被消除
	CC_SYNTHESIZE_READONLY(DisplayMode, m_displayMode, DisplayMode);
	CC_SYNTHESIZE(PriorityLevel, m_sushiPriorityLevel, SushiPriorityLevel);
	void setDisplayMode(DisplayMode mode);
	void applyDisplayMode();

private:
	
};

#endif // __SushiSprite_SCENE_H__
