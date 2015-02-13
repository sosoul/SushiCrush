#include "View/Layer/PlayLayer.h"

#include "Common/ConfigService.h"
#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"
#include "View/Sprite/SushiSprite.h"

namespace {
enum {
	kTagPromptAnimation,
	kTagGuideAnimation,
};

const int kPromptTime = 10.0f;
const int kGuideTime = 1.0f;
}

PlayLayer::PlayLayer(int round) : m_spriteSheet(NULL),
						 m_sushiMatrix(NULL),
						 m_width(0),
						 m_height(0),
						 m_matrixLeftBottomX(0),
						 m_matrixLeftBottomY(0),
						 m_isNeedFillVacancies(false),
						 m_isAnimationing(true),  // start with drop animation
						 m_isTouchEnable(true),
						 m_srcSushi(NULL),
						 m_destSushi(NULL),
						 m_needStopDfs(false),
						 m_round(round),
						 m_isPreDfs(false),
						 m_roundInfo(NULL),
						 m_needRefresh(false),
						 m_isRoundEnded(false),
						 m_isNeedCheck(true),
						 m_isTriggered(false),
						 m_isGuide(false),
						 m_curCrushMode(CRUSH_MODE_NORMAL)
{
	CCASSERT(m_round >= 0 && m_round < TOTAL_ROUND, "");
}

PlayLayer::~PlayLayer()
{
	if (m_sushiMatrix) {
		free(m_sushiMatrix);
	}
	if (m_gridMatrix) {
		free(m_gridMatrix);
	}
	if (m_moveNumMatrix)
	{
		delete m_moveNumMatrix;
		m_moveNumMatrix = nullptr;
	}
	if (m_minEndMoveMatrix)
	{
		delete m_minEndMoveMatrix;
		m_minEndMoveMatrix = nullptr;
	}
	if (m_sushiModeMatrix)
	{
		delete m_sushiModeMatrix;
		m_sushiModeMatrix = nullptr;
	}
	if (m_preDfsMatrix)
	{
		delete m_preDfsMatrix;
		m_preDfsMatrix = nullptr;
	}
	if (m_inDegreeMatrix)
	{
		delete m_inDegreeMatrix;
		m_inDegreeMatrix = nullptr;
	}
	if (m_dfsPathMatrix)
	{
		delete m_dfsPathMatrix;
		m_dfsPathMatrix = nullptr;
	}


	NotificationCenter::getInstance()->removeAllObservers(this);
}

// static 
PlayLayer* PlayLayer::create(int round) {
	PlayLayer* layer = new PlayLayer(round);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	} else {
		delete layer;
		layer = nullptr;
		return layer;
	}
}

bool PlayLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayLayer::onRoundEnd),
		MSG_ROUND_END, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayLayer::onRoundStart),
		MSG_ROUND_START, nullptr);

	// init m_spriteSheet
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_sushiPlist);
	m_spriteSheet = SpriteBatchNode::create(s_sushiRes);

	// Yes, you can load this value from other module.
	m_width = MATRIX_WIDTH;
	m_height = MATRIX_HEIGHT;

	// init position value
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_matrixLeftBottomX = (visibleSize.width - SushiSprite::getContentWidth() * m_width - (m_width - 1) * SUSHI_GAP) / 2;
	m_matrixLeftBottomY = Director::getInstance()->getVisibleOrigin().y + 50;

	// init point array
	int arraySize = sizeof(SushiSprite *) * m_width * m_height;
	m_sushiMatrix = (SushiSprite **)malloc(arraySize);
	memset((void*)m_sushiMatrix, 0, arraySize);
	arraySize = sizeof(GridSprite *) * m_width * m_height;
	m_gridMatrix = (GridSprite **)malloc(arraySize);
	memset((void*)m_gridMatrix, 0, arraySize);

	m_roundInfo = const_cast<RoundInfo*>(ConfigService::getInstance()->getRoundInfo(m_round));

	m_moveNumMatrix = new int[m_width * m_height];
	memset(m_moveNumMatrix, 0, m_width * m_height * sizeof(int));

	m_minEndMoveMatrix = new int[m_width * m_height];
	memset(m_minEndMoveMatrix, 0, m_width * m_height * sizeof(int));

	m_preDfsMatrix = new bool[m_width * m_height];
	memset(m_preDfsMatrix, false, m_width * m_height * sizeof(bool));

	m_sushiModeMatrix = new int[m_width * m_height];
	memset(m_sushiModeMatrix, 0, m_width * m_height * sizeof(int));

	m_inDegreeMatrix = new int[m_width * m_height];
	memset(m_inDegreeMatrix, 0, m_width * m_height * sizeof(int));

	m_dfsPathMatrix = new int[m_width * m_height * m_width * m_height];
	memset(m_dfsPathMatrix, 0, m_width * m_height * m_width * m_height * sizeof(int));

	initMatrix();

	scheduleUpdate();
	scheduleOnce((SEL_SCHEDULE)&PlayLayer::Prompt, kPromptTime);

	// bind touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

void PlayLayer::initMatrix()
{
	int minCol = m_width - 1;
	int maxCol = 0;
	int minRow = m_height - 1;
	int maxRow = 0;
	
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if (isValidGrid(row, col)) {
				if (col < minCol)
					minCol = col;
				if (col > maxCol)
					maxCol = col;

				if (row < minRow)
					minRow = row;
				if (row > maxRow)
					maxRow = row;
			}
		}
	}

	float middleCol = (maxCol + minCol) / 2.0f;
	int widthOffset = m_width / 2;
	m_matrixLeftBottomX -= (middleCol - m_width / 2) * (SushiSprite::getContentWidth() + SUSHI_GAP);

	float middleRow = (maxRow + minRow) / 2.0f;
	int heightOffset = m_height / 2;
	m_matrixLeftBottomY -= (middleCol - m_height / 2) * (SushiSprite::getContentWidth() + SUSHI_GAP);

	std::string key = "round" + StringUtils::toString(m_round);
	if (m_round < ConfigService::getInstance()->guideCount() &&
		!UserDefault::sharedUserDefault()->getBoolForKey(key.c_str()))
		m_isGuide = true;

	Node* stencil = Node::create();
	
	// init sushi and grid matrix
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			createGrid(row, col, stencil);
			if (isValidGrid(row, col)) {
				std::deque<SushiDropPathInfo> dropPath;
				dropPath.push_back(SushiDropPathInfo(row * m_width + col, DFS_DIR_NONE));
				createAndDropSushi(&dropPath, row, col, true, m_isGuide);
			}
		}
	}

	if (m_round < ConfigService::getInstance()->guideCount() && m_isGuide)
		UserDefault::sharedUserDefault()->setBoolForKey(key.c_str(), true);

	m_clipper = ClippingNode::create();
	m_clipper->retain();
	m_clipper->setStencil(stencil);
	m_clipper->setAnchorPoint(Vec2(0.5, 0.5));
	m_clipper->setPosition(0, 0);
	addChild(m_clipper);

	m_clipper->addChild(m_spriteSheet);
}

void PlayLayer::createGrid(int row, int col, Node* stencil) {  //  pass
	if (!isValidRow(row) || !isValidCol(col) || !m_roundInfo)
		return;

	GridType type = m_roundInfo->_matrix[m_width*row + col];
	GridSprite* grid = NULL;
	if (GIRD_TYPE_NONE != type) {
		grid = GridSprite::create(row, col, type);
		grid->setPosition(positionOfItem(row, col));
		m_spriteSheet->addChild(grid);
	}
	m_gridMatrix[row * m_width + col] = grid;

	if (m_roundInfo->_clipper[m_width*row + col]) {
		grid = GridSprite::create(row, col, GIRD_TYPE_NORMAL);
		grid->setPosition(positionOfItem(row, col));
		stencil->addChild(grid);
	}
}

SushiSprite *PlayLayer::sushiOfPoint(Point *point)  // pass
{
	SushiSprite *sushi = NULL;
	Rect rect = Rect(0, 0, 0, 0);

	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_sushiMatrix[i];
		if (sushi) {
			rect.origin.x = sushi->getPositionX() - (sushi->getContentSize().width / 2);
			rect.origin.y = sushi->getPositionY() - (sushi->getContentSize().height / 2);
			rect.size = sushi->getContentSize();
			if (rect.containsPoint(*point)) {
				return sushi;
			}
		}
	}

	return NULL;
}

void PlayLayer::Prompt(float time) {

	stopAllPromptAnimation();
	if (GameController::getInstance()->getCurCrushMode() != CRUSH_MODE_NORMAL)
	{
		return;
	}
	std::vector<SushiSprite*> sushis;
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if (!isLock(row, col, &sushis)) {
				playPromptAnimation(sushis);
				return;
			}
		}
	}
}

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused)  // pass
{
	stopAllPromptAnimation();
	unschedule((SEL_SCHEDULE)&PlayLayer::Prompt);
	if (m_isGuide)
		stopGuideAnimation();
	m_srcSushi = NULL;
	m_destSushi = NULL;
	if (m_isTouchEnable) {
		auto location = touch->getLocation();
		m_srcSushi = sushiOfPoint(&location);
	}
	return m_isTouchEnable;
}

void PlayLayer::onTouchEnded(Touch *touch, Event *unused)
{
	unschedule((SEL_SCHEDULE)&PlayLayer::Prompt);
	scheduleOnce((SEL_SCHEDULE)&PlayLayer::Prompt, 10.0f);
}

void PlayLayer::onTouchMoved(Touch *touch, Event *unused)  // pass
{
	if (m_isRoundEnded)
	{
		return;
	}
	if (GameController::getInstance()->getCurCrushMode() != CRUSH_MODE_NORMAL)
	{
		return;
	}
	if (m_curCrushMode != CRUSH_MODE_NORMAL)
	{
		return;
	}

	if (!m_srcSushi || !m_isTouchEnable) {
		return;
	}

	int row = m_srcSushi->getRow();
	int col = m_srcSushi->getCol();

	auto location = touch->getLocation();
	auto halfSushiWidth = m_srcSushi->getContentSize().width / 2;
	auto halfSushiHeight = m_srcSushi->getContentSize().height / 2;

	auto  upRect = Rect(m_srcSushi->getPositionX() - halfSushiWidth,
		m_srcSushi->getPositionY() + halfSushiHeight,
		m_srcSushi->getContentSize().width,
		m_srcSushi->getContentSize().height);

	if (upRect.containsPoint(location)) {
		++row;
		if (row < m_height) {
			m_destSushi = m_sushiMatrix[row * m_width + col];
		}
		swapSushi();
		return;
	}

	auto  downRect = Rect(m_srcSushi->getPositionX() - halfSushiWidth,
		m_srcSushi->getPositionY() - (halfSushiHeight * 3),
		m_srcSushi->getContentSize().width,
		m_srcSushi->getContentSize().height);

	if (downRect.containsPoint(location)) {
		--row;
		if (row >= 0) {
			m_destSushi = m_sushiMatrix[row * m_width + col];
		}
		swapSushi();
		return;
	}

	auto  leftRect = Rect(m_srcSushi->getPositionX() - (halfSushiWidth * 3),
		m_srcSushi->getPositionY() - halfSushiHeight,
		m_srcSushi->getContentSize().width,
		m_srcSushi->getContentSize().height);

	if (leftRect.containsPoint(location)) {
		--col;
		if (col >= 0) {
			m_destSushi = m_sushiMatrix[row * m_width + col];
		}
		swapSushi();
		return;
	}

	auto  rightRect = Rect(m_srcSushi->getPositionX() + halfSushiWidth,
		m_srcSushi->getPositionY() - halfSushiHeight,
		m_srcSushi->getContentSize().width,
		m_srcSushi->getContentSize().height);

	if (rightRect.containsPoint(location)) {
		col++;
		if (col < m_width) {
			m_destSushi = m_sushiMatrix[row * m_width + col];
		}
		swapSushi();
		return;
	}
}

void PlayLayer::swapSushi()
{
	m_isTouchEnable = false;
	if (!m_srcSushi || !m_destSushi) {
		return;
	}

	Point posOfSrc = m_srcSushi->getPosition();
	Point posOfDest = m_destSushi->getPosition();
	float time = 0.2;

	// 1.swap in matrix
	m_sushiMatrix[m_srcSushi->getRow() * m_width + m_srcSushi->getCol()] = m_destSushi;
	m_sushiMatrix[m_destSushi->getRow() * m_width + m_destSushi->getCol()] = m_srcSushi;
	int tmpRow = m_srcSushi->getRow();
	int tmpCol = m_srcSushi->getCol();
	m_srcSushi->setRow(m_destSushi->getRow());
	m_srcSushi->setCol(m_destSushi->getCol());
	m_destSushi->setRow(tmpRow);
	m_destSushi->setCol(tmpCol);

	// 2.check for removeable
	std::list<SushiSprite *> colChainListOfFirst;
	getColChain(m_srcSushi, colChainListOfFirst);

	std::list<SushiSprite *> rowChainListOfFirst;
	getRowChain(m_srcSushi, rowChainListOfFirst);

	std::list<SushiSprite *> colChainListOfSecond;
	getColChain(m_destSushi, colChainListOfSecond);

	std::list<SushiSprite *> rowChainListOfSecond;
	getRowChain(m_destSushi, rowChainListOfSecond);

	if (colChainListOfFirst.size() >= 3
		|| rowChainListOfFirst.size() >= 3
		|| colChainListOfSecond.size() >= 3
		|| rowChainListOfSecond.size() >= 3
		|| m_srcSushi->getSushiType() == SUSHI_TYPE_5_LINE
		|| m_destSushi->getSushiType() == SUSHI_TYPE_5_LINE
		|| ((m_destSushi->getSushiType() != SUSHI_TYPE_NORMAL && m_srcSushi->getSushiType() != SUSHI_TYPE_NORMAL)
		&& !(m_destSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE && m_srcSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE)
		&& !(m_destSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE && m_srcSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE)
		)) {
		if (GameController::getInstance()->getCurCrushMode() == CRUSH_MODE_NORMAL)
		{
			GameController::getInstance()->onSwapSushiCompleted();
		}
		// just swap
		playSwapAnimation(true);
		return;
	}

	// 3.no chain, swap back
	m_sushiMatrix[m_srcSushi->getRow() * m_width + m_srcSushi->getCol()] = m_destSushi;
	m_sushiMatrix[m_destSushi->getRow() * m_width + m_destSushi->getCol()] = m_srcSushi;
	tmpRow = m_srcSushi->getRow();
	tmpCol = m_srcSushi->getCol();
	m_srcSushi->setRow(m_destSushi->getRow());
	m_srcSushi->setCol(m_destSushi->getCol());
	m_destSushi->setRow(tmpRow);
	m_destSushi->setCol(tmpCol);

	playSwapAnimation(false);
}

// static
bool PlayLayer::isValidRow(int row) {
	return row >= 0 && row < MATRIX_HEIGHT;
}

// static
bool PlayLayer::isValidCol(int col) {
	return col >= 0 && col < MATRIX_WIDTH;
}

Point PlayLayer::positionOfItem(int row, int col)
{
	float x = m_matrixLeftBottomX + (SushiSprite::getContentWidth() + SUSHI_GAP) * col + SushiSprite::getContentWidth() / 2;
	float y = m_matrixLeftBottomY + (SushiSprite::getContentWidth() + SUSHI_GAP) * row + SushiSprite::getContentWidth() / 2;

	return Point(x, y);
}

void PlayLayer::update(float dt)
{
	// check if animationing
	if (m_isAnimationing) {
		// init with false
		m_isAnimationing = false;
		for (int i = 0; i < m_height * m_width; i++) {
			SushiSprite *sushi = m_sushiMatrix[i];
			if (sushi && sushi->getNumberOfRunningActions() > 0) {
				m_isAnimationing = true;
				break;
			}
		}
	}

	// if sushi is moving, ignore use touch event
	m_isTouchEnable = !m_isAnimationing;

	if (!m_isAnimationing) {
		if (m_isRoundEnded) {
			GameController::getInstance()->onRoundEnd();
			m_isRoundEnded = false;
			m_isNeedCheck = false;
		}

		if (m_isNeedFillVacancies) {
			//爆炸效果结束后才掉落新寿司，增强打击感
			fillVacancies();
			m_isNeedFillVacancies = false;
		}
		else if (m_isNeedCheck){
			if (GameController::getInstance()->getCurCrushMode() != CRUSH_MODE_NORMAL)
			{
				triggerCrush();
			}
			else
			{
				checkAndRemoveChain();
			}
		}
	}
}

void PlayLayer::triggerCrush()
{
	int sushiType5LineNum = getSpecialSushiNum(SUSHI_TYPE_5_LINE);
	if (sushiType5LineNum > 0)
	{
		if (m_destSushi == nullptr && m_srcSushi == nullptr)
		{
			SushiSprite *sushi;
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if (sushi->getSushiType() == SUSHI_TYPE_5_LINE)
				{
					if (sushi->getIsNeedRemove())
					{
						checkAndRemoveChain();
						break;
					}

					SushiSprite *targetSushi = getNeighborSushi(sushi);
					if (!targetSushi)
					{
						sushi->setIsNeedRemove(true);
						checkAndRemoveChain();
					}
					else
					{
						m_destSushi = targetSushi;
						m_srcSushi = sushi;
					}
					break;
				}
			}
		}
		else
		{
			checkAndRemoveChain();
		}
	}
	else
	{
		if (GameController::getInstance()->getCurCrushMode() == CRUSH_MODE_REMOVE_SPECIAL_SUSHI)
		{
			int specialSushiNum = getSpecialSushiNum();
			if (specialSushiNum > 0)
			{
				SushiSprite *sushi;
				for (int i = 0; i < m_height * m_width; i++) {
					sushi = m_sushiMatrix[i];
					if (!sushi) {
						continue;
					}
					if (sushi->getSushiType() != SUSHI_TYPE_NORMAL)
					{
						sushi->setIsNeedRemove(false);
						markRemove(sushi);
					}
				}
				checkAndRemoveChain();
			}
			else
			{
				if (!hasNoCrush())
				{
					checkAndRemoveChain();
				}
				else
				{
					if (m_curCrushMode != CRUSH_MODE_REMOVE_SPECIAL_SUSHI)
					{
						return;
					}

					const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
					if (roundInfo.m_leftMoves > 0)
					{
						m_curCrushMode = CRUSH_MODE_GENERATE_SPECIAL_SUSHI;
						GameController::getInstance()->onCrushBegin();
					}
					else
					{
						m_curCrushMode = CRUSH_MODE_NORMAL;
						m_isRoundEnded = true;
					}
				}
			}
		}
		else
		{
			const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
			if (roundInfo.m_leftMoves > 0)
			{
				if (m_isTriggered)
				{
					return;
				}
				m_isTriggered = true;
				SushiSprite *sushi;
				int count = 0;
				for (int i = 0; i < m_height * m_width; i++) {
					sushi = m_sushiMatrix[i];
					if (!sushi) {
						continue;
					}

					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						count++;
					}
				}

				int sushiIndex = rand() % count + 1;

				for (int i = 0; i < m_height * m_width; i++) {
					sushi = m_sushiMatrix[i];
					if (!sushi) {
						continue;
					}

					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						sushiIndex--;
						if (sushiIndex == 0)
						{
							generateSuperSushi(sushi->getRow(), sushi->getCol());
							break;
						}
					}
				}
			}
			else
			{
				int specialSushiNum = getSpecialSushiNum();
				if (specialSushiNum > 0)
				{
				
					SushiSprite *sushi;
					for (int i = 0; i < m_height * m_width; i++) {
					sushi = m_sushiMatrix[i];
					if (!sushi) {
					continue;
					}
					if (sushi->getSushiType() != SUSHI_TYPE_NORMAL)
					{
					sushi->setIsNeedRemove(false);
					markRemove(sushi);
					}
					}
					checkAndRemoveChain();
				}
				else
				{
					if (!hasNoCrush())
					{
						checkAndRemoveChain();
					}
				}
			}
		}
	}
}

SushiSprite* PlayLayer::getNeighborSushi(SushiSprite* sushi)
{
	if (!sushi)
	{
		return nullptr;
	}
	int row = sushi->getRow();
	int col = sushi->getCol();

	SushiSprite* targetSushi = nullptr;
	//按照上下左右的顺序找相邻sushi
	if (isValidRow(row + 1) && isValidGrid(row + 1, col))
	{
		int index = (row + 1) * m_width + col;
		targetSushi = m_sushiMatrix[index];
		if (targetSushi != nullptr)
		{
			return targetSushi;
		}
	}
	if (isValidRow(row - 1) && isValidGrid(row - 1, col))
	{
		int index = (row - 1) * m_width + col;
		targetSushi = m_sushiMatrix[index];
		if (targetSushi != nullptr)
		{
			return targetSushi;
		}
	}
	if (isValidCol(col - 1) && isValidGrid(row, col - 1))
	{
		int index = row * m_width + col - 1;
		targetSushi = m_sushiMatrix[index];
		if (targetSushi != nullptr)
		{
			return targetSushi;
		}
	}
	if (isValidCol(col + 1) && isValidGrid(row, col + 1))
	{
		int index = row * m_width + col + 1;
		targetSushi = m_sushiMatrix[index];
		if (targetSushi != nullptr)
		{
			return targetSushi;
		}
	}

	return targetSushi;
}

void PlayLayer::checkAndRemoveChain()
{
	SushiSprite *sushi;
	// 1. reset ingnore flag
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_sushiMatrix[i];
		if (!sushi) {
			continue;
		}
		sushi->setSushiPriorityLevel(PRIORITY_NORMAL);
		sushi->setIgnoreCheck(false);
	}
	// 分两种情况:
	// 第一种是有src和dist sushi的时候，标记相关生成和移除然后移除
	// 第二种是没有的时候 ，先标记各种需要移除的情况 ，然后设置移除（其中新生成的要设置ignorecheak 同时该设置needremove的也要设置needremove）
	if (m_srcSushi && m_destSushi)
	{
		if (m_destSushi->getSushiType() == SUSHI_TYPE_5_LINE && m_srcSushi->getSushiType() == SUSHI_TYPE_5_LINE)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(true);

			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(true);

			//两个5消产生的互换 消除全部sushi
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				sushi->setIsNeedRemove(false);
				markRemove(sushi);
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_destSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE && m_srcSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE)
		{
			markRemove(m_destSushi);
			markRemove(m_srcSushi);
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_srcSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE && m_destSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE)
		{
			markRemove(m_destSushi);
			markRemove(m_srcSushi);
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_srcSushi->getSushiType() == SUSHI_TYPE_5_LINE && (m_destSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE || m_destSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE))
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(true);

			//5消和四消交换 把同种类型sushi全部变为四消类型并消除
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if (sushi->getImgIndex() == m_destSushi->getImgIndex())
				{
					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						sushi->setSushiType((rand() % 2 == 0) ? SUSHI_TYPE_4_VERTICAL_LINE : SUSHI_TYPE_4_HORIZONTAL_LINE);
						sushi->applySushiType();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_destSushi->getSushiType() == SUSHI_TYPE_5_LINE && (m_srcSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE || m_srcSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE))
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(true);

			//5消和四消交换 把同种类型sushi全部变为四消类型并消除
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if (sushi->getImgIndex() == m_srcSushi->getImgIndex())
				{
					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						sushi->setSushiType((rand() % 2 == 0) ? SUSHI_TYPE_4_VERTICAL_LINE : SUSHI_TYPE_4_HORIZONTAL_LINE);
						sushi->applySushiType();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_srcSushi->getSushiType() == SUSHI_TYPE_5_LINE && m_destSushi->getSushiType() == SUSHI_TYPE_5_CROSS)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(true);

			//5消和T形交换 把同种类型sushi全部变为T型并消除
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if (sushi->getImgIndex() == m_destSushi->getImgIndex())
				{
					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						sushi->setSushiType(SUSHI_TYPE_5_CROSS);
						sushi->applySushiType();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_destSushi->getSushiType() == SUSHI_TYPE_5_LINE && m_srcSushi->getSushiType() == SUSHI_TYPE_5_CROSS)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(true);

			//5消和T形交换 把同种类型sushi全部变为T型并消除
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if (sushi->getImgIndex() == m_srcSushi->getImgIndex())
				{
					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						sushi->setSushiType(SUSHI_TYPE_5_CROSS);
						sushi->applySushiType();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_destSushi->getSushiType() == SUSHI_TYPE_5_CROSS && m_srcSushi->getSushiType() == SUSHI_TYPE_5_CROSS)
		{
			//两个T形和T形交换 消除周围 5*5区域
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if( (sushi->getCol() <= (m_destSushi->getCol() + 2) && sushi->getCol() >= (m_destSushi->getCol() - 2))
					&& (sushi->getRow() <= (m_destSushi->getRow() + 2) && sushi->getRow() >= (m_destSushi->getRow() - 2)))
				{
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_destSushi->getSushiType() == SUSHI_TYPE_5_CROSS && (m_srcSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE || m_srcSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE) )
		{
			//T形和4消交换 消除周围 三行三列
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if ((sushi->getCol() <= (m_destSushi->getCol() + 1) && sushi->getCol() >= (m_destSushi->getCol() - 1))
					|| (sushi->getRow() <= (m_destSushi->getRow() + 1) && sushi->getRow() >= (m_destSushi->getRow() - 1)))
				{
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_srcSushi->getSushiType() == SUSHI_TYPE_5_CROSS && (m_destSushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE || m_destSushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE))
		{
			//T形和4消交换 消除周围 三行三列
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if ((sushi->getCol() <= (m_srcSushi->getCol() + 1) && sushi->getCol() >= (m_srcSushi->getCol() - 1))
					|| (sushi->getRow() <= (m_srcSushi->getRow() + 1) && sushi->getRow() >= (m_srcSushi->getRow() - 1)))
				{
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_srcSushi->getSushiType() == SUSHI_TYPE_5_LINE && m_destSushi->getSushiType() != SUSHI_TYPE_5_LINE)
		{
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if (sushi->getImgIndex() == m_destSushi->getImgIndex())
				{
					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						play5LineSushiTriggerAnimation(m_srcSushi->getPosition(), sushi->getPosition());
					}
				}
			}
			m_srcSushi->setIsNeedRemove(false);
			markRemove(m_srcSushi);

			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_destSushi->getSushiType() == SUSHI_TYPE_5_LINE && m_srcSushi->getSushiType() != SUSHI_TYPE_5_LINE)
		{
			for (int i = 0; i < m_height * m_width; i++) {
				sushi = m_sushiMatrix[i];
				if (!sushi) {
					continue;
				}
				if (sushi->getImgIndex() == m_srcSushi->getImgIndex())
				{
					if (sushi->getSushiType() == SUSHI_TYPE_NORMAL)
					{
						play5LineSushiTriggerAnimation(m_destSushi->getPosition(), sushi->getPosition());
					}
				}
			}


			m_destSushi->setIsNeedRemove(false);
			markRemove(m_destSushi);

			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}


		std::list<SushiSprite*> colChainList;
		getColChain(m_srcSushi, colChainList);

		std::list<SushiSprite*> rowChainList;
		getRowChain(m_srcSushi, rowChainList);

		SushiSprite* sushiTemp;
		std::list<SushiSprite*>::iterator itList;

		if (colChainList.size() >= 3)
		{
			for (itList = colChainList.begin(); itList != colChainList.end(); ++itList) {
				sushiTemp = (SushiSprite*)*itList;
				
				markRemove(sushiTemp);
			}
		}
		if (rowChainList.size() >= 3)
		{
			for (itList = rowChainList.begin(); itList != rowChainList.end(); ++itList) {
				sushiTemp = (SushiSprite *)*itList;
				
				markRemove(sushiTemp);
			}
		}

		typedef std::map<SushiSprite*, SushiType> SushiModeMap;
		SushiModeMap superSushiModeMap;

		if (colChainList.size() >= 5 || rowChainList.size() >= 5)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setSushiType(SUSHI_TYPE_5_LINE);
			m_srcSushi->setIsNeedRemove(false);
			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, SUSHI_TYPE_5_LINE));
		}
		else if (colChainList.size() >= 3 && rowChainList.size() >= 3)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(false);
			m_srcSushi->setSushiType(SUSHI_TYPE_5_CROSS);

			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, SUSHI_TYPE_5_CROSS));
		}
		else if (colChainList.size() >= 4)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(false);
			m_srcSushi->setSushiType(SUSHI_TYPE_4_HORIZONTAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, SUSHI_TYPE_4_HORIZONTAL_LINE));
		}
		else if (rowChainList.size() >= 4)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(false);
			m_srcSushi->setSushiType(SUSHI_TYPE_4_VERTICAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, SUSHI_TYPE_4_VERTICAL_LINE));
		}

		colChainList.clear();
		rowChainList.clear();

		getColChain(m_destSushi, colChainList);
		getRowChain(m_destSushi, rowChainList);
		
		if (colChainList.size() >= 3)
		{
			for (itList = colChainList.begin(); itList != colChainList.end(); itList++) {
				sushiTemp = (SushiSprite *)*itList;
				
				markRemove(sushiTemp);
			}
		}
		if (rowChainList.size() >= 3)
		{
			for (itList = rowChainList.begin(); itList != rowChainList.end(); itList++) {
				sushiTemp = (SushiSprite *)*itList;
				
				markRemove(sushiTemp);
			}
		}

		if (colChainList.size() >= 5 || rowChainList.size() >= 5)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setSushiType(SUSHI_TYPE_5_LINE);
			m_destSushi->setIsNeedRemove(false);
			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, SUSHI_TYPE_5_LINE));
		}
		else if (colChainList.size() >= 3 && rowChainList.size() >= 3)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(false);
			m_destSushi->setSushiType(SUSHI_TYPE_5_CROSS);

			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, SUSHI_TYPE_5_CROSS));
		}
		else if (colChainList.size() >= 4)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(false);
			m_destSushi->setSushiType(SUSHI_TYPE_4_HORIZONTAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, SUSHI_TYPE_4_HORIZONTAL_LINE));
		}
		else if (rowChainList.size() >= 4)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(false);
			m_destSushi->setSushiType(SUSHI_TYPE_4_VERTICAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, SUSHI_TYPE_4_VERTICAL_LINE));
		}

		removeSushi();
		for (SushiModeMap::iterator it = superSushiModeMap.begin(); it != superSushiModeMap.end(); ++it) {
			it->first->applySushiType();
		}

		m_srcSushi = nullptr;
		m_destSushi = nullptr;

	}
	else
	{
		typedef std::map<SushiSprite*, SushiType> SushiModeMap;
		SushiModeMap superSushiModeMap;

		for (int i = 0; i < m_height * m_width; i++) {
			sushi = m_sushiMatrix[i];
			if (!sushi) {
				continue;
			}

			if (sushi->getIsNeedRemove()) {
				markRemove(sushi);
			}
		}

		for (int i = 0; i < m_height * m_width; i++) {
			sushi = m_sushiMatrix[i];
			if (!sushi) {
				continue;
			}

			std::list<SushiSprite *> colChainList;
			getColChain(sushi, colChainList);

			std::list<SushiSprite *> rowChainList;
			getRowChain(sushi, rowChainList);

			if (colChainList.size() < 3 && rowChainList.size() < 3)
			{
				continue;
			}

			SushiSprite *sushiTemp;
			std::list<SushiSprite *>::iterator itList;
			if (colChainList.size() >= 3)
			{
				for (itList = colChainList.begin(); itList != colChainList.end(); itList++) {
					sushiTemp = (SushiSprite *)*itList;
					markRemove(sushiTemp);
				}
			}

			if (rowChainList.size() >= 3)
			{
				for (itList = rowChainList.begin(); itList != rowChainList.end(); itList++) {
					sushiTemp = (SushiSprite *)*itList;
					markRemove(sushiTemp);
				}
			}

			PriorityLevel pRow = getChainMaxPriority(rowChainList);
			PriorityLevel pCol = getChainMaxPriority(colChainList);

			if (colChainList.size() >= 5)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setSushiType(SUSHI_TYPE_5_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_5_LINE);
				setChainMaxPriority(colChainList, PRIORITY_5_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, SUSHI_TYPE_5_LINE));
			}
			else if (rowChainList.size() >= 5)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setSushiType(SUSHI_TYPE_5_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_5_LINE);
				setChainMaxPriority(rowChainList, PRIORITY_5_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, SUSHI_TYPE_5_LINE));
			}
			else if (colChainList.size() >= 3 && rowChainList.size() >= 3 && (int)pRow < (int)PRIORITY_5_CROSS && (int)pCol < (int)PRIORITY_5_CROSS)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setSushiType(SUSHI_TYPE_5_CROSS);
				sushi->setSushiPriorityLevel(PRIORITY_5_CROSS);

				setChainMaxPriority(rowChainList, PRIORITY_5_CROSS);
				setChainMaxPriority(colChainList, PRIORITY_5_CROSS);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, SUSHI_TYPE_5_CROSS));
			}
			else if (colChainList.size() >= 4 && (int)pCol < (int)PRIORITY_4_LINE)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setSushiType(SUSHI_TYPE_4_HORIZONTAL_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_4_LINE);
				setChainMaxPriority(colChainList, PRIORITY_4_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, SUSHI_TYPE_4_HORIZONTAL_LINE));
			}
			else if (rowChainList.size() >= 4 && (int)pRow < (int)PRIORITY_4_LINE)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setSushiType(SUSHI_TYPE_4_VERTICAL_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_4_LINE);
				setChainMaxPriority(rowChainList, PRIORITY_4_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, SUSHI_TYPE_4_VERTICAL_LINE));
			}
		}
		removeSushi();
		for (SushiModeMap::iterator it = superSushiModeMap.begin(); it != superSushiModeMap.end(); ++it) {
			it->first->applySushiType();
		}
	}
}

bool PlayLayer::hasNoCrush()
{
	SushiSprite *sushi;
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_sushiMatrix[i];
		if (!sushi) {
			continue;
		}

		if (sushi->getIsNeedRemove()) {
			return false;
		}

		// start count chain
		std::list<SushiSprite *> colChainList;
		getColChain(sushi, colChainList);

		std::list<SushiSprite *> rowChainList;
		getRowChain(sushi, rowChainList);

		std::list<SushiSprite *> &longerList = colChainList.size() > rowChainList.size() ? colChainList : rowChainList;
		if (longerList.size() >= 3) {
			return false;
		}
	}
	return true;
}

int PlayLayer::getSpecialSushiNum()
{
	int num = 0;
	SushiSprite *sushi;
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_sushiMatrix[i];
		if (!sushi) {
			continue;
		}
		if (sushi->getSushiType() != SUSHI_TYPE_NORMAL)
		{
			num++; 
		}
	}
	return num;
}

int PlayLayer::getSpecialSushiNum(SushiType sushiType)
{
	int num = 0;
	SushiSprite *sushi;
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_sushiMatrix[i];
		if (!sushi) {
			continue;
		}
		if (sushi->getSushiType() == sushiType)
		{
			num++;
		}
	}
	return num;
}

void PlayLayer::getColChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList)
{
	if (sushi->getSushiType() == SUSHI_TYPE_5_LINE)
		return;

	chainList.push_back(sushi);// add first sushi

	int neighborCol = sushi->getCol() - 1;
	while (neighborCol >= 0) {
		SushiSprite *neighborSushi = m_sushiMatrix[sushi->getRow() * m_width + neighborCol];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex()
			&& neighborSushi->getSushiType() != SUSHI_TYPE_5_LINE)) {
			chainList.push_back(neighborSushi);
			neighborCol--;
		}
		else {
			break;
		}
	}

	neighborCol = sushi->getCol() + 1;
	while (neighborCol < m_width) {
		SushiSprite *neighborSushi = m_sushiMatrix[sushi->getRow() * m_width + neighborCol];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& neighborSushi->getSushiType() != SUSHI_TYPE_5_LINE) {
			chainList.push_back(neighborSushi);
			neighborCol++;
		}
		else {
			break;
		}
	}
}

void PlayLayer::setChainMaxPriority(std::list<SushiSprite *> &chainList, PriorityLevel p)
{
	SushiSprite *sushiTemp;
	std::list<SushiSprite *>::iterator itList;

	for (itList = chainList.begin(); itList != chainList.end(); itList++) {
		sushiTemp = (SushiSprite *)*itList;

		if (sushiTemp->getIgnoreCheck() && !sushiTemp->getIsNeedRemove() && (int)sushiTemp->getSushiPriorityLevel() < int(p))
		{
			sushiTemp->setIgnoreCheck(false);
			sushiTemp->setIsNeedRemove(true);
			sushiTemp->setSushiType(SUSHI_TYPE_NORMAL);
		}
		sushiTemp->setSushiPriorityLevel(p);
	}
}

PriorityLevel PlayLayer::getChainMaxPriority(std::list<SushiSprite *> &chainList)
{
	SushiSprite *sushiTemp;
	std::list<SushiSprite *>::iterator itList;

	PriorityLevel p = PRIORITY_NORMAL;

	for (itList = chainList.begin(); itList != chainList.end(); itList++) {
		sushiTemp = (SushiSprite *)*itList;
		if ((int)sushiTemp->getSushiPriorityLevel() > (int)p)
		{
			p = sushiTemp->getSushiPriorityLevel();
		}
	}
	return p;
}

void PlayLayer::getRowChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList)
{
	if (sushi->getSushiType() == SUSHI_TYPE_5_LINE)
		return;

	chainList.push_back(sushi);// add first sushi

	int neighborRow = sushi->getRow() - 1;
	while (neighborRow >= 0) {
		SushiSprite *neighborSushi = m_sushiMatrix[neighborRow * m_width + sushi->getCol()];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& neighborSushi->getSushiType() != SUSHI_TYPE_5_LINE) {
			chainList.push_back(neighborSushi);
			neighborRow--;
		}
		else {
			break;
		}
	}

	neighborRow = sushi->getRow() + 1;
	while (neighborRow < m_height) {
		SushiSprite *neighborSushi = m_sushiMatrix[neighborRow * m_width + sushi->getCol()];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& neighborSushi->getSushiType() != SUSHI_TYPE_5_LINE) {
			chainList.push_back(neighborSushi);
			neighborRow++;
		}
		else {
			break;
		}
	}
}

void PlayLayer::removeSushi()
{
	int score = 0;
	MapTarget map;
	initTargetMap(&map);
	for (int i = 0; i < m_height * m_width; i++) {
		SushiSprite *sushi = m_sushiMatrix[i];
		if (!sushi) {
			continue;
		}

		if (sushi->getIgnoreCheck())
		{
			continue;
		}

		if (sushi->getIsNeedRemove()) {
			m_isNeedFillVacancies = true;  // 需要掉落
			// TODO: 检查类型，并播放一行消失的动画
			if (sushi->getSushiType() == SUSHI_TYPE_5_LINE) {
				playExplode5LineSushiAnimation(sushi);
			} else if (sushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE)
			{
				playExplode4HorizonytalLineSushiAnimation(sushi);
			} else if (sushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE)
			{
				playExplode4VerticalLineSushiAnimation(sushi);
			} else if (sushi->getSushiType() == SUSHI_TYPE_5_CROSS) {
				playExplode5CrossSushiAnimation(sushi);
			}
			switch (sushi->getImgIndex())
			{
			case 0:
			{
				int value = map.at(TARGET_TYPE_SUSHI_1);
				map.at(TARGET_TYPE_SUSHI_1) = value + 1;
			}
				break;
			case 1:
			{
				int value = map.at(TARGET_TYPE_SUSHI_2);
				map.at(TARGET_TYPE_SUSHI_2) = value + 1;
			}
				break;
			case 2:
			{
				int value = map.at(TARGET_TYPE_SUSHI_3);
				map.at(TARGET_TYPE_SUSHI_3) = value + 1;
			}
				break;
			case 3:
			{
				int value = map.at(TARGET_TYPE_SUSHI_4);
				map.at(TARGET_TYPE_SUSHI_4) = value + 1;
			}
				break;
			case 4:
			{
				int value = map.at(TARGET_TYPE_SUSHI_5);
				map.at(TARGET_TYPE_SUSHI_5) = value + 1;
			}
				break;
			case 5:
			{
				int value = map.at(TARGET_TYPE_SUSHI_6);
				map.at(TARGET_TYPE_SUSHI_6) = value + 1;
			}
				break;
			default:
				break;
			}
			explodeSushi(sushi, &score, &map);
		}
	}
	if (score)
		map[TARGET_TYPE_SCORE] = score;

	if (map[TARGET_TYPE_SCORE] || map[TARGET_TYPE_SUSHI_1] ||
		map[TARGET_TYPE_SUSHI_2] || map[TARGET_TYPE_SUSHI_3] ||
		map[TARGET_TYPE_SUSHI_4] || map[TARGET_TYPE_SUSHI_5] ||
		map[TARGET_TYPE_SUSHI_6] || map[TARGET_TYPE_JELLY] ||
		map[TARGET_TYPE_DOUBLE_JELLY] || map[TARGET_TYPE_CREAM] ||
		map[TARGET_TYPE_DOUBLE_CREAM]) {
		if (!m_needRefresh)
		{
			GameController::getInstance()->onRemoveSushiCompleted(map);
		}
	}
	else {
		if (isLock())
		{
			refresh();
		}
	}
}

void PlayLayer::didPlayExplodeSushiAnimation(SushiSprite *sushi)
{
	if (!sushi)
		return;
	m_sushiMatrix[sushi->getRow() * m_width + sushi->getCol()] = NULL;
	if (m_srcSushi == sushi) {
		m_isTouchEnable = false;
		m_srcSushi = NULL;
	}
	if (m_destSushi == sushi) {
		m_isTouchEnable = false;
		m_destSushi = NULL;
	}
		
	sushi->removeFromParent();
}

void PlayLayer::explodeSushi(SushiSprite *sushi, int* score, MapTarget* map)
{
	if (!sushi)
		return;

	playExplodeSushiAnimation(sushi);
	
	if (!m_needRefresh)
	{
		playAddScoreAnimation(sushi);
		(*score) += sushi->getScore();

		// deal with grids
		int row = sushi->getRow();
		int col = sushi->getCol();
		GridSprite* grid = m_gridMatrix[row*m_width + col];
		changeGridType(grid, getGridType(row, col), false, map);

		if (isValidRow(row - 1)) {
			changeGridType(m_gridMatrix[(row - 1)*m_width + col], getGridType(row - 1, col), true, map);
		}
		if (isValidCol(col + 1)) {
			changeGridType(m_gridMatrix[row*m_width + (col + 1)], getGridType(row, col + 1), true, map);
		}
		if (isValidRow(row + 1)) {
			changeGridType(m_gridMatrix[(row + 1)*m_width + col], getGridType(row + 1, col), true, map);
		}
		if (isValidCol(col - 1)) {
			changeGridType(m_gridMatrix[row*m_width + (col - 1)], getGridType(row, col - 1), true, map);
		}
	}
}

void PlayLayer::changeGridType(GridSprite* grid, GridType type, bool isNeighbor, MapTarget* map) {
	if (!grid)
		return;
	if (isNeighbor) {
		switch (type)
		{
		case GRID_TYPE_CREAM:
			grid->setGridType(GIRD_TYPE_NORMAL);
			break;
		case GRID_TYPE_DOUBLE_CREAM:
			grid->setGridType(GRID_TYPE_CREAM);
			break;
		default:
			break;
		}
	} else {
		switch (type)
		{
		case GRID_TYPE_JELLY:
		{
			grid->setGridType(GIRD_TYPE_NORMAL);
			TargetType origin_target_type = TARGET_TYPE_SCORE;
			switch (grid->getOriginGridType())
			{
			case GRID_TYPE_JELLY:
				origin_target_type = TARGET_TYPE_JELLY;
				break;
			case GRID_TYPE_DOUBLE_JELLY:
				origin_target_type = TARGET_TYPE_DOUBLE_JELLY;
				break;
			case GRID_TYPE_CREAM:
				origin_target_type = TARGET_TYPE_CREAM;
				break;
			case GRID_TYPE_DOUBLE_CREAM:
				origin_target_type = TARGET_TYPE_DOUBLE_CREAM;
				break;
			default:
				break;
			}
			if (TARGET_TYPE_SCORE != origin_target_type) {
				int value = map->at(origin_target_type);
				map->at(origin_target_type) = value + 1;
			}
		}
			break;
		case GRID_TYPE_DOUBLE_JELLY:
		{
			grid->setGridType(GRID_TYPE_JELLY);
		}
			break;
		default:
			break;
		}
	}
	
}

bool PlayLayer::canCreateNewSushi(int index)
{
	return ConfigService::getInstance()->isProducer(m_round, getRowByIndex(index), getColByIndex(index));

}

int PlayLayer::getRowByIndex(int index)
{
	return index / m_width; 
}

int PlayLayer::getColByIndex(int index)
{
	return index % m_width;
}

//深搜获取路径 如果顶部位置有sushi那么用现有sushi 否则掉落新sushi
bool PlayLayer::dfs(std::deque<SushiDropPathInfo>* dropPath, bool* visited) {
	if (!dropPath || !visited)
		return false;
	if (dropPath->empty())
		return false;

	int index = dropPath->back().m_sushiIndex;
	DfsSearchDirection direction = dropPath->back().m_direction;
	CCASSERT(direction != DFS_DIR_NONE, "Invalid direction in dfs()!");

	int row = getRowByIndex(index);
	int col = getColByIndex(index);

	int curIndex = -1;

	if (direction == DFS_DIR_DIRECT)
	{
		curIndex = ConfigService::getInstance()->getPortalSrc(m_round, row, col);
		row = getRowByIndex(curIndex);
		col = getColByIndex(curIndex);
	}
	else
	{
		row++;
		if (DFS_DIR_LEFT == direction)
			--col;
		else if (DFS_DIR_RIGHT == direction)
			++col;
		curIndex = row * m_width + col;
	}

	if (-1 == curIndex)
		return false;

	bool isVisited = visited[curIndex];
	bool validIndex = isValidCol(col) && isValidRow(row);
	bool validGrid = isValidGrid(row, col);
	if (isVisited || !validIndex || !validGrid){
		if (!isVisited && validIndex && !validGrid)
			visited[curIndex] = true;
		if (direction == DFS_DIR_RIGHT || direction == DFS_DIR_DIRECT)
		{
			dropPath->pop_back();
			if (dropPath->empty())
				return false;

			DfsSearchDirection curDirection = dropPath->back().m_direction;

			if (curDirection == DFS_DIR_MIDDLE)
			{
				curDirection = DFS_DIR_LEFT;
			}
			else if (curDirection == DFS_DIR_LEFT)
			{
				curDirection = DFS_DIR_RIGHT;
			}
			*(&dropPath->back().m_direction) = curDirection;
			return dfs(dropPath, visited);
		}
		else if (direction == DFS_DIR_LEFT)
		{
			*(&dropPath->back().m_direction) = DFS_DIR_RIGHT;
			return dfs(dropPath, visited);
		}
		else if (direction == DFS_DIR_MIDDLE)
		{
			*(&dropPath->back().m_direction) = DFS_DIR_LEFT;
			return dfs(dropPath, visited);
		}
	}

	if (!m_isPreDfs)
	{
		SushiSprite *sushi = m_sushiMatrix[curIndex];
		if (NULL != sushi)
		{
			dropPath->push_back(SushiDropPathInfo(curIndex, DFS_DIR_NONE));
			return true;
		}
	}
	else
	{
		if (m_preDfsMatrix[curIndex])
		{
			dropPath->push_back(SushiDropPathInfo(curIndex, DFS_DIR_NONE));
			return true;
		}
	}

	if (canCreateNewSushi(curIndex))
	{
		dropPath->push_back(SushiDropPathInfo(curIndex, DFS_DIR_NONE));
		return true;
	}

	if (direction == DFS_DIR_RIGHT || direction == DFS_DIR_DIRECT)
	{
		visited[curIndex] = true;
	}

	if (ConfigService::getInstance()->getPortalSrc(m_round, row, col) != -1)
	{
		direction = DFS_DIR_DIRECT;
	}
	else
	{
		direction = DFS_DIR_MIDDLE;
	}
	dropPath->push_back(SushiDropPathInfo(curIndex, direction));

	return dfs(dropPath, visited);
}

int PlayLayer::getMinEndMove(int row, int col)
{
	int minEndMove = 0;

	int indexBefore = ConfigService::getInstance()->getPortalDest(m_round, row, col);

	if (indexBefore != -1)
	{
		if (m_minEndMoveMatrix[indexBefore] > minEndMove)
		{
			minEndMove = m_minEndMoveMatrix[indexBefore];
		}
	}
	else if (isValidRow(row - 1) && isValidCol(col) && isValidGrid(row - 1, col) && m_minEndMoveMatrix[(row - 1) * m_width + col] > minEndMove)
	{
		minEndMove = m_minEndMoveMatrix[(row - 1) * m_width + col];
	}
	return minEndMove;
}

void PlayLayer::setMoveNum(const std::deque<SushiDropPathInfo>& dropPath, int row, int col)
{
	int minEndMove = getMinEndMove(row, col);
	if (m_moveNumMatrix[dropPath.back().m_sushiIndex] + dropPath.size() - 1 > minEndMove)
	{
		m_minEndMoveMatrix[row * m_width + col] = m_moveNumMatrix[dropPath.back().m_sushiIndex] + dropPath.size() - 1;
	}
	else
	{
		m_minEndMoveMatrix[row * m_width + col] = minEndMove;
		m_moveNumMatrix[dropPath.back().m_sushiIndex] = minEndMove - (dropPath.size() - 1);
	}
}

void PlayLayer::fillVacancies(int row, int col)
{
	if (!isValidGrid(row, col)){
		return;
	}
	bool needFind = false;

	if (m_isPreDfs)
	{
		if (!m_preDfsMatrix[row * m_width + col])
		{
			needFind = true;
		}
	}
	else
	{
		if (NULL == m_sushiMatrix[row * m_width + col])
		{
			needFind = true;
		}
	}

	if (!needFind)
		return;

	if (canCreateNewSushi(row * m_width + col))
	{
		m_needStopDfs = false;
		if (!m_isPreDfs)
		{
			std::deque<SushiDropPathInfo> dropPath;
			dropPath.push_back(SushiDropPathInfo(row * m_width + col, DFS_DIR_NONE));
			createAndDropSushi(&dropPath, row, col);
		}
		else
		{
			m_preDfsMatrix[row * m_width + col] = true;
		}
	}
	else
	{
		// 此时需要深搜获取掉落路径 深搜结果有三种
		// 1 没有可以用来掉落的路径，此时格子为空
		// 2 有可以用来掉落的路径，且用于填充格子的寿司是其他地方已经存在的
		// 3 有可以用来掉落的路径，且用于填充格子的寿司是新生成的
			
		bool *visited = new bool[m_height * m_width];
		memset(visited, 0, sizeof(bool)* m_height * m_width);
		std::deque<SushiDropPathInfo> dropPath;
		SushiDropPathInfo item;

		if (ConfigService::getInstance()->getPortalSrc(m_round, row, col) != -1)
		{
			item.m_direction = DFS_DIR_DIRECT;
		}
		else
		{
			item.m_direction = DFS_DIR_MIDDLE;
		}
		item.m_sushiIndex = row * m_width + col;
		dropPath.push_back(item);
		visited[row * m_width + col] = true;
		bool canDrop = dfs(&dropPath, visited);

		if (canDrop)
		{
			m_needStopDfs = false;
			if (!m_isPreDfs)
			{
				SushiSprite *newSushi = m_sushiMatrix[dropPath.back().m_sushiIndex];
				if (newSushi)
				{
					setMoveNum(dropPath, row, col);

					//寿司移动
					dropExistSushi(&dropPath, row, col);
				}
				else
				{
					//掉落新寿司
					m_moveNumMatrix[dropPath.back().m_sushiIndex] = m_moveNumMatrix[dropPath.back().m_sushiIndex] + 1;
					setMoveNum(dropPath, row, col);

					createAndDropSushi(&dropPath, row, col);
				}
			}
			else
			{
				//由sushiStack进行拓扑排序

				int sourceIndex = dropPath.back().m_sushiIndex;

				if (!canCreateNewSushi(sourceIndex))
				{
					m_preDfsMatrix[sourceIndex] = false;
				}

				int destIndex = 0;
				dropPath.pop_back();
				while (!dropPath.empty())
				{
					destIndex = dropPath.back().m_sushiIndex;

					if (m_dfsPathMatrix[destIndex * (m_height * m_width) + sourceIndex] == 0)
					{
						m_dfsPathMatrix[destIndex * (m_height * m_width) + sourceIndex] = 1;
						++m_inDegreeMatrix[sourceIndex];
					}
					dropPath.pop_back();
					sourceIndex = destIndex;
				}

				m_preDfsMatrix[row * m_width + col] = true;
			}
		}
		delete visited;
	}
}

void PlayLayer::fillVacancies()
{
	if (m_needRefresh) {
		for (int row = 0; row < m_height; row++) {
			for (int col = 0; col < m_width; col++) {
				if (isValidGrid(row, col)) {
					std::deque<SushiDropPathInfo> dropPath;
					dropPath.push_back(SushiDropPathInfo(row * m_width + col, DFS_DIR_NONE));
					createAndDropSushi(&dropPath, row, col);
				}
			}
		}
		m_needRefresh = false;
		return;
	}
	// 1. drop exist sushi
	memset(m_moveNumMatrix, 0, m_width * m_height * sizeof(int));
	memset(m_minEndMoveMatrix, 0, m_width * m_height * sizeof(int));
	memset(m_preDfsMatrix, 0, m_width * m_height * sizeof(bool));
	memset(m_inDegreeMatrix, 0, m_width * m_height * sizeof(int));
	memset(m_dfsPathMatrix, 0, m_width * m_height * m_width * m_height * sizeof(int));
	m_isPreDfs = true;
	for (int i = 0; i < m_width*m_height; i++)
	{
		if (NULL != m_sushiMatrix[i])
		{
			m_preDfsMatrix[i] = true;
		}
	}

	m_needStopDfs = false;

	while (!m_needStopDfs)
	{
		m_needStopDfs = true;
		for (int row = 0; row < m_height; ++row)
		{
			for (int col = 0; col < m_width / 2; ++col)
			{
				fillVacancies(row, col);
				fillVacancies(row, m_width - 1 - col);
			}

			if (0 != m_width % 2)
				fillVacancies(row, m_width / 2);
		}
	}


	m_isPreDfs = false;

	std::deque<int> searchStack;
	int searchCount = 0;

	for (int i = 0; i < m_width*m_height; i++)
	{
		int tempRow = getRowByIndex(i);
		int tempCol = getColByIndex(i);
		if (m_inDegreeMatrix[i] == 0)
		{
			searchStack.push_back(i);
		}
	}

	while (searchStack.size() != 0)
	{
		int curSearchIndex = searchStack.back();

		int tempRow = getRowByIndex(curSearchIndex);
		int tempCol = getColByIndex(curSearchIndex);

		fillVacancies(getRowByIndex(curSearchIndex), getColByIndex(curSearchIndex));
		searchStack.pop_back();
		searchCount++;

		for (int i = curSearchIndex * m_width * m_height; i < (curSearchIndex + 1) * m_width * m_height; i++)
		{
			if (m_dfsPathMatrix[i] == 1)
			{
				int destIndex = i % (m_width * m_height);

				m_dfsPathMatrix[i] = 0;
				m_inDegreeMatrix[destIndex] --;

				if (m_inDegreeMatrix[destIndex] == 0)
				{
					searchStack.push_back(destIndex);
				}

			}
		}
	}

	CCASSERT(searchCount == m_width * m_height, "Tiled map config error!");

	m_needRefresh = false;

	if (hasNoCrush())
	{
		const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();

		int specialSushiNum = getSpecialSushiNum();

		CrushMode crushMode = GameController::getInstance()->getCurCrushMode();

		if (crushMode == CRUSH_MODE_NORMAL)
		{
			if (GameController::getInstance()->isPass(m_round))
			{
				m_curCrushMode = CRUSH_MODE_REMOVE_SPECIAL_SUSHI;
				GameController::getInstance()->onTargetCompleted();
			}
			else
			{
				if (0 == roundInfo.m_leftMoves)
					m_isRoundEnded = true;
			}
		}
		else if (crushMode == CRUSH_MODE_REMOVE_SPECIAL_SUSHI)
		{
			if (roundInfo.m_leftMoves > 0 && specialSushiNum == 0)
			{
				m_curCrushMode = CRUSH_MODE_GENERATE_SPECIAL_SUSHI;
				GameController::getInstance()->onCrushBegin();
			}
			else if (roundInfo.m_leftMoves == 0 && specialSushiNum == 0)
			{
				m_isRoundEnded = true;
			}
		}
		else if (crushMode == CRUSH_MODE_GENERATE_SPECIAL_SUSHI)
		{
			if (specialSushiNum == 0 && roundInfo.m_leftMoves == 0)
			{
				m_isRoundEnded = true;
			}
		}
	}
}

void PlayLayer::dropExistSushi(std::deque<SushiDropPathInfo>* dropPath, int rowDest, int colDest) {
	if (!dropPath)
		return;

	int startIndex = dropPath->back().m_sushiIndex;
	int row = getRowByIndex(startIndex);
	int col = getColByIndex(startIndex);

	SushiSprite *sushi = m_sushiMatrix[row * m_width + col];
	m_sushiMatrix[rowDest * m_width + colDest] = sushi;
	m_sushiMatrix[row * m_width + col] = NULL;
	sushi->setRow(rowDest);
	sushi->setCol(colDest);
	Point startPosition = sushi->getPosition();
	sushi->stopAllActions();  // must stop pre drop action
	playDropAnimation(sushi, dropPath, false);
}

void PlayLayer::createAndDropSushi(std::deque<SushiDropPathInfo>* dropPath, int row, int col, bool isInit, bool isGuide)
{
	if (!dropPath)
		return;

	int topImgIndex = -1;
	int leftImgIndex = -1;
	int imgIndex = -1;

	if (isInit) {  // if it is an initial round, make the sushis stay un-eliminated status.
		if (isGuide) {
			imgIndex = ConfigService::getInstance()->getImageIndexInGuideMap(m_round, row, col);
		}
		
		// scan the sushis on the top
		int topRow1 = row - 1;
		int topRow2 = row - 2;
		if (isValidRow(topRow1) && isValidRow(topRow2)) {
			SushiSprite* topSushi1 = m_sushiMatrix[topRow1*m_width + col];
			SushiSprite* topSushi2 = m_sushiMatrix[topRow2*m_width + col];
			if (topSushi1 && topSushi2 && topSushi1->getImgIndex() == topSushi2->getImgIndex())
				topImgIndex = topSushi1->getImgIndex();
		}

		// scan the sushis on the left
		int leftCol1 = col - 1;
		int leftCol2 = col - 2;
		if (isValidCol(leftCol1) && isValidCol(leftCol2)) {
			SushiSprite* leftSushi1 = m_sushiMatrix[row*m_width + leftCol1];
			SushiSprite* leftSushi2 = m_sushiMatrix[row*m_width + leftCol2];
			if (leftSushi1 && leftSushi2 && leftSushi1->getImgIndex() == leftSushi2->getImgIndex())
				leftImgIndex = leftSushi1->getImgIndex();
		}
	}

	SushiSprite* sushi = NULL;
	if (-1 == imgIndex)
		sushi = SushiSprite::create(row, col, topImgIndex, leftImgIndex, PRIORITY_NORMAL);
	else
		sushi = SushiSprite::create(row, col, imgIndex, SUSHI_TYPE_NORMAL);

	if (m_needRefresh)
	{
		sushi->setSushiType((SushiType)m_sushiModeMatrix[row * m_width + col]);
		sushi->applySushiType();
		playRefreshDropAnimation(sushi);
		return;
	}

	if (isInit)
		sushi->setPosition(positionOfItem(row, col));
	else
		playDropAnimation(sushi, dropPath, true);

	playDropAnimation(sushi, dropPath, true);
	m_spriteSheet->addChild(sushi);
	m_sushiMatrix[row * m_width + col] = sushi;
}

void PlayLayer::markRemove(SushiSprite *sushi)
{
	if (sushi->getIsNeedRemove()) {
		return;
	}
	if (sushi->getIgnoreCheck()) {
		return;
	}

	// mark self
	sushi->setIsNeedRemove(true);

	if (sushi->getSushiType() == SUSHI_TYPE_5_LINE) {  // 所有相同的寿司消除
		int index = -1;
		if (sushi == m_srcSushi && m_destSushi)
			index = m_destSushi->getImgIndex();
		else if (sushi == m_destSushi && m_srcSushi)
			index = m_srcSushi->getImgIndex();
		if (-1 == index)
			return;
		for (int row = 0; row < m_height; row++) {
			for (int col = 0; col < m_width; col++) {
				SushiSprite* tmp = m_sushiMatrix[row*m_width + col];
				if (!tmp || tmp == sushi) {
					continue;
				}
				if (tmp->getImgIndex() == index)
				{
					if (tmp->getSushiType() == SUSHI_TYPE_NORMAL) {
						tmp->setIsNeedRemove(true);
					}
					else if (tmp->getSushiType() != SUSHI_TYPE_NORMAL) {
						markRemove(tmp);
					}
				}
			}
		}
	} else if (sushi->getSushiType() == SUSHI_TYPE_4_VERTICAL_LINE) {
		// check for type and mark for certical neighbour
		for (int row = 0; row < m_height; row++) {
			SushiSprite *tmp = m_sushiMatrix[row * m_width + sushi->getCol()];
			if (!tmp || tmp == sushi) {
				continue;
			}

			if (tmp->getSushiType() == SUSHI_TYPE_NORMAL) {
				tmp->setIsNeedRemove(true);
			}
			else {
				markRemove(tmp);
			}
		}
		
	} else if (sushi->getSushiType() == SUSHI_TYPE_4_HORIZONTAL_LINE) {
		// check for type and mark for horizontal neighbour
		for (int col = 0; col < m_width; col++) {
			SushiSprite *tmp = m_sushiMatrix[sushi->getRow() * m_width + col];
			if (!tmp || tmp == sushi) {
				continue;
			}

			if (tmp->getSushiType() == SUSHI_TYPE_NORMAL) {
				tmp->setIsNeedRemove(true);
			}
			else {
				markRemove(tmp);
			}
		}
	}
	else if (sushi->getSushiType() == SUSHI_TYPE_5_CROSS) {  // 九宫格内的寿司消除
		int startX = sushi->getRow() - 1;
		if (startX < 0) startX = 0;
		int startY = sushi->getCol() - 1;
		if (startY < 0) startY = 0;
		int endX = sushi->getRow() + 1;
		if (endX >= m_width) endX = m_width - 1;
		int endY = sushi->getCol() + 1;
		if (endY >= m_height) endY = m_height - 1;
		for (int row = startX; row <= endX; row++) {
			for (int col = startY; col <= endY; col++) {
				SushiSprite* tmp = m_sushiMatrix[row*m_width + col];
				if (!tmp || tmp == sushi) {
					continue;
				}
				if (tmp->getSushiType() == SUSHI_TYPE_NORMAL) {
					tmp->setIsNeedRemove(true);
				}
				else {
					markRemove(tmp);
				}
			}
		}
	}
}

void PlayLayer::onRoundEnd(Ref* obj) {

}

void PlayLayer::onRoundStart(Ref* obj) {
	return;
	SushiSprite *sushi;
	// 1. reset ingnore flag
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_sushiMatrix[i];
		if (!sushi) {
			continue;
		}
		sushi->setIsNeedRemove(true);
		markRemove(sushi);
	}
}

bool PlayLayer::isValidGrid(int row, int col) {
	if (!isValidRow(row) || !isValidCol(col) || !m_roundInfo)
		return false;
	GridType type = getGridType(row, col);
	return (GIRD_TYPE_NORMAL == type || GRID_TYPE_JELLY == type || GRID_TYPE_DOUBLE_JELLY == type);
}

GridType PlayLayer::getGridType(int row, int col) {
	if (!isValidRow(row) || !isValidCol(col) || !m_gridMatrix[row*m_width + col])
		return GIRD_TYPE_NONE;
	return m_gridMatrix[row*m_width + col]->getGridType();
}

void PlayLayer::refresh() {
	m_needRefresh = true;
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if (SushiSprite* sushi = m_sushiMatrix[row*m_width + col])
			{
				sushi->setIgnoreCheck(false);
				sushi->setIsNeedRemove(true);
				m_sushiModeMatrix[row*m_width + col] = (int)sushi->getSushiType();
				//m_mapRefreshRetain[row*m_width + col] = sushi->getSushiType();
			}
		}
	}
	removeSushi();
}

bool PlayLayer::isLock() {
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if (!isLock(row, col))
				return false;
		}
	}
	return true;
}

bool PlayLayer::canbeRemovedSushis(SushiSprite* sushi1, SushiSprite* sushi2, int imgIndex) {
	if (!sushi1 || !sushi2)
		return false;
	return (sushi1->getImgIndex() == sushi2->getImgIndex() && sushi1->getImgIndex() == imgIndex);
}

void PlayLayer::generateSuperSushi(int row, int col) {
	SushiSprite * sushi = m_sushiMatrix[row * m_width + col];
	int randNum = rand() % 3;
	if (randNum == 0)
	{
		sushi->setSushiType(SUSHI_TYPE_4_VERTICAL_LINE);
	}
	else if (randNum == 1)
	{
		sushi->setSushiType(SUSHI_TYPE_4_HORIZONTAL_LINE);
	}
	else if (randNum == 2)
	{
		sushi->setSushiType(SUSHI_TYPE_5_CROSS);
	}
	playGenerateSuperSushiAnimation(sushi);
}

void PlayLayer::setSushiType(SushiSprite * sushi)
{
	sushi->applySushiType();
	sushi->setIsNeedRemove(false);
	markRemove(sushi);
	GameController::getInstance()->onSwapSushiCompleted();
	m_isTriggered = false;
}

bool PlayLayer::isLock(int row, int col, std::vector<SushiSprite*>* sushis) {
	if (!isValidGrid(row, col))
		return true;
	if (!isValidRow(row) || !isValidCol(col))
		return true;
	SushiSprite* sushi = m_sushiMatrix[row*m_width+col];
	if (!sushi)
		return true;
	int index = sushi->getImgIndex();
	SushiSprite* sushi1 = NULL;
	SushiSprite* sushi2 = NULL;

	// scan the sushis on the top
	int topRow1 = row - 1;
	int topRow2 = row - 2;
	int topRow3 = row - 3;
	int topLeftCol1 = col - 1;
	int topLeftCol2 = col - 2;
	int topRightCol1 = col + 1;
	int topRightCol2 = col + 2;
	if (isValidGrid(topRow1, col)) {
		if (isValidRow(topRow2) && isValidRow(topRow3)) {
			sushi1 = m_sushiMatrix[topRow2*m_width + col];
			sushi2 = m_sushiMatrix[topRow3*m_width + col];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidCol(topLeftCol1) && isValidCol(topLeftCol2) && isValidRow(topRow1)) {
			sushi1 = m_sushiMatrix[topRow1*m_width + topLeftCol1];
			sushi2 = m_sushiMatrix[topRow1*m_width + topLeftCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidCol(topRightCol1) && isValidCol(topRightCol2) && isValidRow(topRow1)) {
			sushi1 = m_sushiMatrix[topRow1*m_width + topRightCol1];
			sushi2 = m_sushiMatrix[topRow1*m_width + topRightCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
	}

	// scan the sushis on the right
	int rightCol1 = col + 1;
	int rightCol2 = col + 2;
	int rightCol3 = col + 3;
	int rightTopRow1 = row - 1;
	int rightTopRow2 = row - 2;
	int rightBottomRow1 = row + 1;
	int rightBottomRow2 = row + 2;
	if (isValidGrid(row, rightCol1)) {
		if (isValidCol(rightCol2) && isValidCol(rightCol3)) {
			sushi1 = m_sushiMatrix[row*m_width + rightCol2];
			sushi2 = m_sushiMatrix[row*m_width + rightCol3];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidRow(rightTopRow1) && isValidRow(rightTopRow2) && isValidCol(rightCol1)) {
			sushi1 = m_sushiMatrix[rightTopRow1*m_width + rightCol1];
			sushi2 = m_sushiMatrix[rightTopRow2*m_width + rightCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidRow(rightBottomRow1) && isValidRow(rightBottomRow2) && isValidCol(rightCol1)) {
			sushi1 = m_sushiMatrix[rightBottomRow1*m_width + rightCol1];
			sushi2 = m_sushiMatrix[rightBottomRow2*m_width + rightCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
	}

	// scan the sushis on the bottom
	int bottomRow1 = row + 1;
	int bottomRow2 = row + 2;
	int bottomRow3 = row + 3;
	int bottomLeftCol1 = col - 1;
	int bottomLeftCol2 = col - 2;
	int bottomRightCol1 = col + 1;
	int bottomRightCol2 = col + 2;
	if (isValidGrid(bottomRow1, col)) {
		if (isValidRow(bottomRow2) && isValidRow(bottomRow3)) {
			sushi1 = m_sushiMatrix[bottomRow2*m_width + col];
			sushi2 = m_sushiMatrix[bottomRow3*m_width + col];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidCol(bottomLeftCol1) && isValidCol(bottomLeftCol2) && isValidRow(bottomRow1)) {
			sushi1 = m_sushiMatrix[bottomRow1*m_width + bottomLeftCol1];
			sushi2 = m_sushiMatrix[bottomRow1*m_width + bottomLeftCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidCol(bottomRightCol1) && isValidCol(bottomRightCol2) && isValidRow(bottomRow1)) {
			sushi1 = m_sushiMatrix[bottomRow1*m_width + bottomRightCol1];
			sushi2 = m_sushiMatrix[bottomRow1*m_width + bottomRightCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
	}

	// scan the sushis on the left
	int leftCol1 = col - 1;
	int leftCol2 = col - 2;
	int leftCol3 = col - 3;
	int leftTopRow1 = row - 1;
	int leftTopRow2 = row - 2;
	int leftBottomRow1 = row + 1;
	int leftBottomRow2 = row + 2;
	if (isValidGrid(row, leftCol1)) {
		if (isValidCol(leftCol2) && isValidCol(leftCol3)) {
			sushi1 = m_sushiMatrix[row*m_width + leftCol2];
			sushi2 = m_sushiMatrix[row*m_width + leftCol3];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidRow(leftTopRow1) && isValidRow(leftTopRow2) && isValidCol(leftCol1)) {
			sushi1 = m_sushiMatrix[leftTopRow1*m_width + leftCol1];
			sushi2 = m_sushiMatrix[leftTopRow2*m_width + leftCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
		if (isValidRow(leftBottomRow1) && isValidRow(leftBottomRow2) && isValidCol(leftCol1)) {
			sushi1 = m_sushiMatrix[leftBottomRow1*m_width + leftCol1];
			sushi2 = m_sushiMatrix[leftBottomRow2*m_width + leftCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index)) {
				if (sushis) {
					sushis->push_back(sushi);
					sushis->push_back(sushi1);
					sushis->push_back(sushi2);
				}
				return false;
			}
		}
	}

	return true;
}

void PlayLayer::didPlayAddScoreAnimation(LabelBMFont* label) {
	label->removeFromParent();
	label->release();
}

void PlayLayer::playSwapAnimation(bool success) {
	if (!m_srcSushi || !m_destSushi)
		return;
	m_isAnimationing = true;

	Point posOfSrc = m_srcSushi->getPosition();
	Point posOfDest = m_destSushi->getPosition();
	float time = 0.2;
	Sequence* srcSequence = nullptr;
	Sequence* destSequence = nullptr;
	if (success) {
		m_srcSushi->runAction(MoveTo::create(time, posOfDest));
		m_destSushi->runAction(MoveTo::create(time, posOfSrc));
	}
	else {
		m_srcSushi->runAction(Sequence::create(
			MoveTo::create(time, posOfDest),
			MoveTo::create(time, posOfSrc),
			NULL));
		m_destSushi->runAction(Sequence::create(
			MoveTo::create(time, posOfSrc),
			MoveTo::create(time, posOfDest),
			NULL));
	}
}

void PlayLayer::playDropAnimation(SushiSprite* sushi, std::deque<SushiDropPathInfo>* dropPath, bool isCreate) {
	if (!sushi || !dropPath)
		return;
	if (dropPath->empty())
		return;

	m_isAnimationing = true;

	int startIndex = dropPath->back().m_sushiIndex;
	int row = getRowByIndex(startIndex);
	int col = getColByIndex(startIndex);
	Point startPosition = positionOfItem(row, col);
	sushi->setPosition(startPosition);
	dropPath->pop_back();

	int lastIndex = startIndex;
	bool needClone = false;
	SushiSprite *sushiClone = SushiSprite::clone(sushi);
	m_spriteSheet->addChild(sushiClone);
	sushiClone->setPosition(positionOfItem(getRowByIndex(lastIndex), getColByIndex(lastIndex)));
	sushiClone->setVisible(false);

	Vector<FiniteTimeAction*> moveVector;
	Vector<FiniteTimeAction*> moveVectorClone;

	if (isCreate) {
		auto position = sushi->getPosition();
		sushi->setPosition(ccpAdd(Vec2(0, SushiSprite::getContentWidth() + SUSHI_GAP), position));
		if (dropPath->empty()) {
			if (m_moveNumMatrix[row * m_width + col] > 0)
				moveVector.pushBack(DelayTime::create(DROP_TIME * m_moveNumMatrix[row * m_width + col]));
		}
		else {
			if (m_moveNumMatrix[startIndex] > 1)
				moveVector.pushBack(DelayTime::create(DROP_TIME * (m_moveNumMatrix[startIndex] - 1)));
		}

		moveVector.pushBack(MoveTo::create(DROP_TIME, position));

		if (m_moveNumMatrix[startIndex] > 0)
			moveVectorClone.pushBack(DelayTime::create(DROP_TIME * (m_moveNumMatrix[startIndex])));
	}
	else
	{
		if (m_moveNumMatrix[startIndex] > 0) {
			moveVector.pushBack(DelayTime::create(DROP_TIME * (m_moveNumMatrix[startIndex])));
			moveVectorClone.pushBack(DelayTime::create(DROP_TIME * (m_moveNumMatrix[startIndex])));
		}
	}

	while (!dropPath->empty())
	{
		int curIndex = dropPath->back().m_sushiIndex;
		DfsSearchDirection dir = dropPath->back().m_direction;
		int curCol = getColByIndex(curIndex);
		int curRow = getRowByIndex(curIndex);
		dropPath->pop_back();

		if (dir != DFS_DIR_DIRECT)
		{
			auto actionMoveTo = MoveTo::create(DROP_TIME, positionOfItem(curRow, curCol));
			moveVector.pushBack(actionMoveTo);

			moveVectorClone.pushBack(DelayTime::create(DROP_TIME));
		}
		else
		{
			moveVector.pushBack(MoveBy::create(DROP_TIME, Vec2(0.0f, (SushiSprite::getContentWidth() + SUSHI_GAP) * -1)));
			auto placeAction = Place::create(positionOfItem(curRow, curCol));
			moveVector.pushBack(placeAction);

			needClone = true;
			moveVectorClone.pushBack(Place::create(positionOfItem(curRow + 1, curCol)));
			moveVectorClone.pushBack(Show::create());
			moveVectorClone.pushBack(MoveTo::create(DROP_TIME, positionOfItem(curRow, curCol)));
			moveVectorClone.pushBack(Hide::create());

		}
		lastIndex = curIndex;
	}

	moveVector.pushBack(MoveBy::create(0.1, Vec2(0, 2)));
	moveVector.pushBack(MoveBy::create(0.1, Vec2(0, -2)));
	auto sequence = Sequence::create(moveVector);
	sushi->runAction(sequence);
	if (needClone)
	{

		auto sequenceClone = Sequence::create(moveVectorClone);
		sushiClone->runAction(sequenceClone);
	}
}

void PlayLayer::playExplode4HorizonytalLineSushiAnimation(SushiSprite* sushi) {
	if (!sushi)
		return;
	m_isAnimationing = true;

	Size size = Director::getInstance()->getVisibleSize();
	float scaleX = 4;
	float scaleY = 0.7;
	float time = 0.3;
	Point point = sushi->getPosition();
	float speed = 0.6f;

	auto colorSpriteRight = Sprite::createWithSpriteFrameName(s_colorHRight);
	addChild(colorSpriteRight, 10);
	Point endPosition1 = Point(point.x - size.width, point.y);
	colorSpriteRight->setPosition(point);
	colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
		NULL));

	auto colorSpriteLeft = Sprite::createWithSpriteFrameName(s_colorLRight);
	addChild(colorSpriteLeft, 10);
	Point endPosition2 = Point(point.x + size.width, point.y);
	colorSpriteLeft->setPosition(point);
	colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
		NULL));
}

void PlayLayer::playExplode4VerticalLineSushiAnimation(SushiSprite* sushi) {
	if (!sushi)
		return;
	m_isAnimationing = true;

	Size size = Director::getInstance()->getWinSize();
	float scaleY = 4;
	float scaleX = 0.7;
	float time = 0.3;
	Point point = sushi->getPosition();
	float speed = 0.6f;

	auto colorSpriteDown = Sprite::createWithSpriteFrameName(s_colorVDown);
	addChild(colorSpriteDown, 10);
	Point endPosition1 = Point(point.x, point.y - size.height);
	colorSpriteDown->setPosition(point);
	colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
		NULL));

	auto colorSpriteUp = Sprite::createWithSpriteFrameName(s_colorVUp);
	addChild(colorSpriteUp, 10);
	Point endPosition2 = Point(point.x, point.y + size.height);
	colorSpriteUp->setPosition(point);
	colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
		NULL));
}

void PlayLayer::playExplode5LineSushiAnimation(SushiSprite* sushi) {
	if (!sushi)
		return;

	m_isAnimationing = true;

	Point point = sushi->getPosition();
	auto explosion = ParticleExplosion::create();
	explosion->setTexture(Director::getInstance()->getTextureCache()->addImage(s_absExploadCross));
	explosion->setPosition(point);

	explosion->setTotalParticles(1000);
	explosion->setLife(2.0f);

	this->addChild(explosion);
}

void PlayLayer::playExplode5CrossSushiAnimation(SushiSprite* sushi) {
	if (!sushi)
		return;

	m_isAnimationing = true;

	Point point = sushi->getPosition();
	auto explosion = ParticleExplosion::create();
	explosion->setTexture(Director::getInstance()->getTextureCache()->addImage(s_absExploadCross));
	explosion->setPosition(point);

	explosion->setTotalParticles(1000);
	explosion->setLife(2.0f);

	this->addChild(explosion);
}

void PlayLayer::playExplodeSushiAnimation(SushiSprite *sushi) {
	if (!sushi)
		return;

	m_isAnimationing = true;
	float time = 0.3;

	// 1. action for sushi
	sushi->runAction(Sequence::create(
		ScaleTo::create(time, 0.0),
		CallFunc::create(CC_CALLBACK_0(PlayLayer::didPlayExplodeSushiAnimation, this, sushi)),
		NULL));

	// 2. action for circle
	auto circleSprite = Sprite::createWithSpriteFrameName(s_circle);
	addChild(circleSprite, 10);
	circleSprite->setPosition(sushi->getPosition());
	circleSprite->setScale(0);// start size
	circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
		NULL));

	// 3. particle effect
	auto particleStars = ParticleSystemQuad::create(s_stars);
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(sushi->getPosition());
	particleStars->setScale(0.3);
	addChild(particleStars, 20);
}

void PlayLayer::playAddScoreAnimation(SushiSprite *sushi) {
	if (!sushi)
		return;

	m_isAnimationing = true;

	// 4. score animation
	Point pos = sushi->getPosition();
	auto label = LabelBMFont::create("+" + StringUtils::toString(sushi->getScore()), "fonts/boundsTestFont.fnt");

	label->retain();
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	label->setPosition(pos);
	addChild(label);

	const float duration = 1.0f;
	Vec2 dstPos = pos + Vec2(0, 10);
	FadeOut* fadeOut = FadeOut::create(duration);
	MoveTo* moveTo = MoveTo::create(duration, dstPos);
	auto spawn = Spawn::create(moveTo, fadeOut, nullptr);
	auto seq = Sequence::create(spawn,
		CallFunc::create(CC_CALLBACK_0(PlayLayer::didPlayAddScoreAnimation, this, label)), nullptr);
	label->runAction(seq);
}

void PlayLayer::play5LineSushiTriggerAnimation(Point start, Point end){
	m_isAnimationing = true;
	auto sp = Sprite::createWithSpriteFrameName(s_starMidDone);
	sp->setScale(0.5f);
	this->addChild(sp);
	sp->setPosition(start);
	sp->runAction(Sequence::create(MoveTo::create(0.5f, end), Hide::create(), nullptr));
}

void PlayLayer::playGenerateSuperSushiAnimation(SushiSprite* sushi) {
	m_isAnimationing = true;
	auto sp = Sprite::createWithSpriteFrameName(s_starMidDone);
	sp->setScale(0.5f);
	this->addChild(sp);
	sp->setPosition(positionOfItem(11, 2));

	sp->runAction(Sequence::create(MoveTo::create(1.0f, sushi->getPosition()),
		Hide::create(),
		CallFunc::create(CC_CALLBACK_0(PlayLayer::setSushiType, this, sushi)),
		nullptr));
}

void PlayLayer::playRefreshDropAnimation(SushiSprite* sushi) {
	if (!sushi)
		return;
	m_isAnimationing = true;
	int row = sushi->getRow();
	int col = sushi->getCol();
	Point endPosition = positionOfItem(row, col);
	Point startPosition = ccpAdd(Vec2(0, (SushiSprite::getContentWidth() + SUSHI_GAP)*m_height / 2), endPosition);
	sushi->setPosition(startPosition);
	MoveTo* moveTo = MoveTo::create(1.5, endPosition);
	sushi->runAction(moveTo);
	m_spriteSheet->addChild(sushi);
	m_sushiMatrix[row * m_width + col] = sushi;
}

void PlayLayer::playPromptAnimation(std::vector<SushiSprite*> sushis) {
	if (sushis.empty())
		return;
	std::vector<SushiSprite*>::iterator it = sushis.begin();
	for (; sushis.end() != it; ++it) {
		ScaleTo* scaleTo1 = ScaleTo::create(0.3f, 1.1f, 0.9f);
		ScaleTo* scaleTo2 = ScaleTo::create(0.3f, 0.9f, 1.1f);
		ScaleTo* scaleTo3 = ScaleTo::create(0.3f, 1.1f, 0.9f);
		ScaleTo* scaleTo4 = ScaleTo::create(0.3f, 1.0f, 1.0f);
		DelayTime* delay = DelayTime::create(1.0f);
		Vector<FiniteTimeAction*> actions;
		actions.pushBack(scaleTo1);
		actions.pushBack(scaleTo2);
		actions.pushBack(scaleTo3);
		actions.pushBack(scaleTo4);
		actions.pushBack(delay);
		RepeatForever* repeat = RepeatForever::create(Sequence::create(actions));
		repeat->setTag(kTagPromptAnimation);
		(*it)->runAction(repeat);
	}
}

void PlayLayer::stopAllPromptAnimation()
{
	for (int i = 0; i < m_height * m_width; i++) {
		SushiSprite* sushi = m_sushiMatrix[i];
		if (!sushi) {
			continue;
		}
		sushi->stopAllActionsByTag(kTagPromptAnimation);
	}
}

void PlayLayer::tryPlayGuideAnimation() {
	if (!m_isGuide)
		return;

	scheduleOnce((SEL_SCHEDULE)&PlayLayer::playGuideAnimation, kGuideTime);
}

void PlayLayer::playGuideAnimation(float time) {	// 先写死吧
	auto clip = ClippingNode::create();
	clip->setInverted(true);
	clip->setAlphaThreshold(0.0f);
	addChild(clip, 10, kTagGuideAnimation);

	auto layerColor = LayerColor::create(Color4B(0, 0, 0, 150));
	clip->addChild(layerColor, 1);

	auto stencil = Node::create();
	SushiSprite* sushi = NULL;
	switch (m_round)
	{
	case 0:
		sushi = SushiSprite::clone(m_sushiMatrix[7 * m_width + 1]);
		sushi->setPosition(positionOfItem(7, 1));
		if (sushi)
			stencil->addChild(sushi);
		sushi = SushiSprite::clone(m_sushiMatrix[7 * m_width + 2]);
		sushi->setPosition(positionOfItem(7, 2));
		if (sushi)
			stencil->addChild(sushi);
		sushi = SushiSprite::clone(m_sushiMatrix[7 * m_width + 3]);
		sushi->setPosition(positionOfItem(7, 3));
		if (sushi)
			stencil->addChild(sushi);
		sushi = SushiSprite::clone(m_sushiMatrix[7 * m_width + 4]);
		sushi->setPosition(positionOfItem(7, 4));
		if (sushi)
			stencil->addChild(sushi);
		break;
	case 1:
		break;
	default:
		break;
	}

	clip->setStencil(stencil);
}

void PlayLayer::stopGuideAnimation() {
	auto guide = getChildByTag(kTagGuideAnimation);
	guide->removeFromParent();
	m_isGuide = false;
}