#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "View/Sprite/GridSprite.h"
#include "View/Sprite/SushiSprite.h"
#include "Common/ConfigService.h"

USING_NS_CC;

#define DROP_TIME 0.3

class PlayLayer : public Layer
{
public:
	enum DfsSearchDirection {
		DFS_DIR_LEFT = 0,
		DFS_DIR_MIDDLE,
		DFS_DIR_RIGHT,
		DFS_DIR_DIRECT,
	};
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

	void refresh();

private:
	SpriteBatchNode *m_spriteSheet;
	SushiSprite **m_sushiMatrix;// 1D array which store (SushiSprite *).
	GridSprite **m_gridMatrix;
	int * m_moveNumMatrix;
	int * m_minEndMoveMatrix;
	int * m_sushiModeMatrix;
	int * m_preDfsMatrix;

	int * m_inDegreeMatrix;
	int * m_dfsPathMatrix;

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
	bool m_needStopDfs;//判断是否需要结束 dfs
	int m_round;
	bool m_isPreDfs;
	RoundInfo* m_roundInfo;
	bool m_needRefresh;
	ClippingNode* m_clipper;

	void initMatrix();
	void createGrid(int row, int col, Node* stencil);
	void createAndDropSushi(int row, int col, bool isInit);
	void moveAction(Node *node, std::deque<int>* sushiStack, std::deque<DfsSearchDirection>* directionStack, int startIndex, bool isCreated);
	void createAndDropSushi(std::deque<int>* sushiStack, std::deque<DfsSearchDirection>* directionStack, int rowNow, int colNow, bool isCreate);

	Point positionOfItem(int row, int col);
	void checkAndRemoveChain();
	PriorityLevel getChainMaxPriority(std::list<SushiSprite *> &chainList);
	void setChainMaxPriority(std::list<SushiSprite *> &chainList, PriorityLevel p);

	void getColChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList);
	void getRowChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList);

	void removeSushi();
	void actionEndCallback(Node *node);
	void explodeSushi(SushiSprite *sushi, int* score, MapTarget* map);

	void explode4HorizonytalLineSushi(Point point);
	void explode4VerticalLineSushi(Point point);
	void explode5LineLineSushi(SushiSprite* sushi);
	void explode5CrossLineSushi(SushiSprite* sushi);
	bool canCreateNewSushi(int index);
	int getRowByIndex(int index);
	int getColByIndex(int index);
	bool dfs(std::deque<int>* sushiStack, int *visited, std::deque<DfsSearchDirection>* direction);
	int getMinEndMove(int row, int col);
	void fillVacancies();
	void setMoveNum(std::deque<int>* sushiStack, int row, int col);
	void fillVacancies(int row, int col);
	SushiSprite *sushiOfPoint(Point *point);
	void swapSushi();
	void markRemove(SushiSprite *sushi);
	bool checkActualRoundEnd();
	bool isValidGrid(int row, int col);
	GridType getGridType(int row, int col);
	static bool isValidRow(int row);
	static bool isValidCol(int col);
	void animation5line(Point start, Point end);
	// is the marix stay lock status.
	bool isLock();
	// is the sushi stay lock status.
	bool isLock(int row, int col);
	bool canbeRemovedSushis(SushiSprite* sushi1, SushiSprite* sushi2, int imgIndex);
	void didShowScoreNumber(LabelBMFont* label);
	void changeGridType(GridSprite* grid, GridType type, bool isNeighbor, MapTarget* map);
	
};

#endif // __PLAY_LAYER_H__
