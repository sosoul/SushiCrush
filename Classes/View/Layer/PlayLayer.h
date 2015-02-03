#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "View/Sprite/GridSprite.h"
#include "View/Sprite/SushiSprite.h"
#include "Common/ConfigService.h"

USING_NS_CC;

#define DROP_TIME 0.19

class PlayLayer : public Layer
{
public:
	enum DfsSearchDirection {
		DFS_DIR_NONE = 0,
		DFS_DIR_LEFT,
		DFS_DIR_MIDDLE,
		DFS_DIR_RIGHT,
		DFS_DIR_DIRECT,
	};

	struct SushiDropPathInfo
	{
		int m_sushiIndex;
		DfsSearchDirection m_direction;

		SushiDropPathInfo() : m_sushiIndex(0), m_direction(DFS_DIR_NONE) {}
		SushiDropPathInfo(int sushiIndex, DfsSearchDirection direction) :
			m_sushiIndex(sushiIndex), m_direction(direction) {}
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
	SpriteBatchNode* m_spriteSheet;
	SushiSprite** m_sushiMatrix;// 1D array which store (SushiSprite *).
	GridSprite** m_gridMatrix;
	int* m_moveNumMatrix;
	int* m_minEndMoveMatrix;
	typedef std::map<int, SushiType> RefreshRetainMap;
	RefreshRetainMap m_mapRefreshRetain;
	int* m_sushiModeMatrix;  // std::map<int, int>

	bool* m_preDfsMatrix;
	int* m_inDegreeMatrix;
	int* m_dfsPathMatrix;

	int m_width;
	int m_height;
	// for rapid count position
	float m_matrixLeftBottomX;
	float m_matrixLeftBottomY;
	bool m_isTouchEnable;  // is need to deal with touch event
	SushiSprite *m_srcSushi;  // 用户移动的寿司
	SushiSprite *m_destSushi;  // 移动到哪个寿司
	bool m_isAnimationing;
	bool m_isRoundEnded;
	bool m_isNeedCheck;
	bool m_isNeedFillVacancies;  //是否有空缺需要填补
	bool m_needStopDfs;  //判断是否需要结束 dfs
	int m_round;
	bool m_isPreDfs;
	RoundInfo* m_roundInfo;
	bool m_needRefresh;
	bool m_isTriggered;
	CrushMode m_curCrushMode;
	ClippingNode* m_clipper;

	void initMatrix();
	void createGrid(int row, int col, Node* stencil);

	// basic utility methods
	int getRowByIndex(int index);
	int getColByIndex(int index);
	int getSpecialSushiNum();
	int getSpecialSushiNum(SushiType sushiType);
	Point positionOfItem(int row, int col);
	GridType getGridType(int row, int col);
	SushiSprite* sushiOfPoint(Point* point);
	bool canCreateNewSushi(int index);
	bool isValidGrid(int row, int col);
	static bool isValidRow(int row);
	static bool isValidCol(int col);
	void setSushiType(SushiSprite * sushi);
	void getColChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList);
	void getRowChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList);
	PriorityLevel getChainMaxPriority(std::list<SushiSprite *> &chainList);
	bool hasNoCrush();
	bool canbeRemovedSushis(SushiSprite* sushi1, SushiSprite* sushi2, int imgIndex);
	void markRemove(SushiSprite* sushi);
	void setChainMaxPriority(std::list<SushiSprite *> &chainList, PriorityLevel p);
	void checkAndRemoveChain();
	void changeGridType(GridSprite* grid, GridType type, bool isNeighbor, MapTarget* map);
	void fillVacancies();
	void fillVacancies(int row, int col);
	SushiSprite* getNeighborSushi(SushiSprite* sushi);
	void Prompt(float time);

	// play scene status
	bool isLock();  // is the matrix stay lock status?
	bool isLock(int row, int col, std::vector<SushiSprite*>* sushis = NULL);  // is the sushi which with row and col stay lock status?
	void triggerCrush();
	

	// sushi related methods
	void createAndDropSushi(std::deque<SushiDropPathInfo>* dropPath, int row, int col, bool isInit = false);
	void dropExistSushi(std::deque<SushiDropPathInfo>* dropPath, int row, int col);
	void swapSushi();
	void removeSushi();
	void explodeSushi(SushiSprite *sushi, int* score, MapTarget* map);
	void generateSuperSushi(int row, int col);

	// dfs related methods
	bool dfs(std::deque<SushiDropPathInfo>* dropPath, bool* visited);
	int getMinEndMove(int row, int col);
	void setMoveNum(const std::deque<SushiDropPathInfo>& dropPath, int row, int col);

	// animations
	void playSwapAnimation(bool success);
	void playDropAnimation(SushiSprite* sushi, std::deque<SushiDropPathInfo>* dropPath, bool isCreate);
	void playExplode4HorizonytalLineSushiAnimation(SushiSprite* sushi);
	void playExplode4VerticalLineSushiAnimation(SushiSprite* sushi);
	void playExplode5LineSushiAnimation(SushiSprite* sushi);
	void playExplode5CrossSushiAnimation(SushiSprite* sushi);
	void playExplodeSushiAnimation(SushiSprite *sushi);
	void playAddScoreAnimation(SushiSprite *sushi);
	void play5LineSushiTriggerAnimation(Point start, Point end);
	void playGenerateSuperSushiAnimation(SushiSprite* sushi);
	void playRefreshDropAnimation(SushiSprite* sushi);
	void playPromptAnimation(std::vector<SushiSprite*> sushis);

	// stop repeat animations
	void stopAllPromptAnimation();

	// animation callbacks
	void didPlayExplodeSushiAnimation(SushiSprite *sushi);
	void didPlayAddScoreAnimation(LabelBMFont* label);

};

#endif // __PLAY_LAYER_H__
