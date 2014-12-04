#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class SushiSprite;

class PlayLayer : public Layer
{
public:
	PlayLayer(int round);
	~PlayLayer();
	static PlayLayer* create(int round);

	virtual bool init() override;
	virtual void update(float dt) override;
	virtual bool onTouchBegan(Touch *touch, Event *unused) override;
	virtual void onTouchMoved(Touch *touch, Event *unused) override;

	// notifications
	void onRoundEnd(Ref* obj);
	void onRoundStart(Ref* obj);

private:
	SpriteBatchNode *m_spriteSheet;
	SushiSprite **m_matrix;// 1D array which store (SushiSprite *).
	int m_width;
	int m_height;
	// for rapid count position
	float m_matrixLeftBottomX;
	float m_matrixLeftBottomY;
	bool m_isTouchEnable;// is need to deal with touch event
	SushiSprite *m_srcSushi;// 用户移动的寿司
	SushiSprite *m_destSushi;// 移动到哪个寿司
	bool m_isAnimationing;
	bool m_isNeedFillVacancies;//是否有空缺需要填补
	bool m_movingVertical;// true: 4消除产生纵向炸弹.  false: 4消除产生横向炸弹.
	int m_round;

	void initMatrix();
	void createAndDropSushi(int row, int col, bool isInit);
	Point positionOfItem(int row, int col);
	void checkAndRemoveChain();
	void getColChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList);
	void getRowChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList);
	void removeSushi();
	void actionEndCallback(Node *node);
	void explodeSushi(SushiSprite *sushi);

	void explodeSpecialH(Point point);
	void explodeSpecialV(Point point);

	void fillVacancies();
	SushiSprite *sushiOfPoint(Point *point);
	void swapSushi();
	void markRemove(SushiSprite *sushi);
	bool checkActualRoundEnd();
	bool hasSushi(int row, int col);
	int getFirstValidRows(int col);
	static bool isValidRow(int row);
	static bool isValidCol(int col);
	
};

#endif // __PLAY_LAYER_H__
