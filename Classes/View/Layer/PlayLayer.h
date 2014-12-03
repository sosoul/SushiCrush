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
	SushiSprite *m_srcSushi;// �û��ƶ�����˾
	SushiSprite *m_destSushi;// �ƶ����ĸ���˾
	bool m_isAnimationing;
	bool m_isNeedFillVacancies;//�Ƿ��п�ȱ��Ҫ�
	bool m_movingVertical;// true: 4������������ը��.  false: 4������������ը��.
	int m_round;

	void initMatrix();
	void createAndDropSushi(int row, int col);
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
	
};

#endif // __PLAY_LAYER_H__
