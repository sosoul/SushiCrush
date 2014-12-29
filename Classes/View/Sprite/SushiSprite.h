#ifndef __SPRITE_SUSHI_SPRITE_H__
#define __SPRITE_SUSHI_SPRITE_H__

#include "cocos2d.h"
#include "Common/Types.h"

USING_NS_CC;

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
	CC_SYNTHESIZE_READONLY(SushiType, m_sushiType, SushiType);
	CC_SYNTHESIZE(int, m_score, Score);
	CC_SYNTHESIZE(PriorityLevel, m_sushiPriorityLevel, SushiPriorityLevel);
	void setSushiType(SushiType mode);
	void applySushiType();

private:
	
};

#endif // __SPRITE_SUSHI_SPRITE_H__
