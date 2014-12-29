#include "View/Layer/PlayLayer.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"
#include "View/Sprite/SushiSprite.h"

namespace {
// TODO: The matrix data need to be read from config file.

const int kRoundMatrixes[10][8][7] = { { { 1, 3, 3, 3, 3, 3, 1 },
										 { 1, 1, 4, 1, 4, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 2, 2, 2, 1, 1 },
										 { 1, 1, 2, 5, 2, 1, 1 },
										 { 1, 1, 2, 2, 2, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 } }, // 1

									   { { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 3, 3, 3, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 } },  // 2

									  { { 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 3

									  { { 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 4

									  { { 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 5

									  { { 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 6

									  { { 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 7

									  { { 0, 0, 0, 0, 0, 0, 0 },
										{ 0, 0, 1, 1, 1, 0, 0 },
										{ 0, 1, 1, 1, 1, 1, 0 },
										{ 0, 1, 1, 1, 1, 1, 0 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 8

									  { { 0, 0, 0, 0, 0, 0, 0 },
										{ 0, 0, 1, 1, 1, 0, 0 },
										{ 0, 1, 1, 1, 1, 1, 0 },
										{ 0, 1, 1, 1, 1, 1, 0 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 9

									  { { 0, 0, 0, 0, 0, 0, 0 },
										{ 0, 0, 1, 1, 1, 0, 0 },
										{ 0, 1, 1, 1, 1, 1, 0 },
										{ 0, 1, 1, 1, 1, 1, 0 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 } },  // 10
};
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
						 m_movingVertical(true),  // drop animation is vertical
						 m_round(round),
						 m_needRefresh(false)
{
	CCASSERT(m_round > 0 && m_round <= TOTAL_ROUND, "");
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

	// background
	Size winSize = Director::getInstance()->getWinSize();

	// init m_spriteSheet
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_sushiPlist);
	m_spriteSheet = SpriteBatchNode::create(s_sushiRes);
	addChild(m_spriteSheet);

	// Yes, you can load this value from other module.
	m_width = MATRIX_WIDTH;
	m_height = MATRIX_HEIGHT;

	// init position value
	m_matrixLeftBottomX = (winSize.width - SushiSprite::getContentWidth() * m_width - (m_width - 1) * SUSHI_GAP) / 2;
	m_matrixLeftBottomY = 5;// (winSize.height - SushiSprite::getContentWidth() * m_height - (m_height - 1) * SUSHI_GAP) / 2;

	// init point array
	int arraySize = sizeof(SushiSprite *) * m_width * m_height;
	m_sushiMatrix = (SushiSprite **)malloc(arraySize);
	memset((void*)m_sushiMatrix, 0, arraySize);
	arraySize = sizeof(GridSprite *) * m_width * m_height;
	m_gridMatrix = (GridSprite **)malloc(arraySize);
	memset((void*)m_gridMatrix, 0, arraySize);

	m_moveNumMatrix = new int[m_width * m_height];
	memset(m_moveNumMatrix, 0, m_width * m_height * sizeof(int));

	m_minEndMoveMatrix = new int[m_width * m_height];
	memset(m_minEndMoveMatrix, 0, m_width * m_height * sizeof(int));

	m_sushiModeMatrix = new int[m_width * m_height];
	memset(m_sushiModeMatrix, 0, m_width * m_height * sizeof(int));

	initMatrix();
	scheduleUpdate();

	// bind touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}

void PlayLayer::initMatrix()
{
	// init sushi and grid matrix
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			createGrid(row, col);
			if (isValidGrid(row, col))
				createAndDropSushi(row, col, true);
		}
	}
}

void PlayLayer::createGrid(int row, int col) {
	if (!isValidRow(row) || !isValidCol(col))
		return;
	GridType type = (GridType)(kRoundMatrixes[m_round-1][row][col]);
	GridSprite* grid = NULL;
	if (GIRD_TYPE_NONE != type) {
		grid = GridSprite::create(row, col, type);
		grid->setPosition(positionOfItem(row, col));
		addChild(grid);
	}
	m_gridMatrix[row * m_width + col] = grid;
}

SushiSprite *PlayLayer::sushiOfPoint(Point *point)
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

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused)
{
	m_srcSushi = NULL;
	m_destSushi = NULL;
	if (m_isTouchEnable) {
		auto location = touch->getLocation();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		location -= origin;
		m_srcSushi = sushiOfPoint(&location);
	}
	return m_isTouchEnable;
}

void PlayLayer::onTouchMoved(Touch *touch, Event *unused)
{
	if (!m_srcSushi || !m_isTouchEnable) {
		return;
	}

	int row = m_srcSushi->getRow();
	int col = m_srcSushi->getCol();

	auto location = touch->getLocation();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	location -= origin;
	auto halfSushiWidth = m_srcSushi->getContentSize().width / 2;
	auto halfSushiHeight = m_srcSushi->getContentSize().height / 2;

	auto  upRect = Rect(m_srcSushi->getPositionX() - halfSushiWidth,
		m_srcSushi->getPositionY() + halfSushiHeight,
		m_srcSushi->getContentSize().width,
		m_srcSushi->getContentSize().height);

	if (upRect.containsPoint(location)) {
		row++;
		if (row < m_height) {
			m_destSushi = m_sushiMatrix[row * m_width + col];
		}
		m_movingVertical = true;
		swapSushi();
		return;
	}

	auto  downRect = Rect(m_srcSushi->getPositionX() - halfSushiWidth,
		m_srcSushi->getPositionY() - (halfSushiHeight * 3),
		m_srcSushi->getContentSize().width,
		m_srcSushi->getContentSize().height);

	if (downRect.containsPoint(location)) {
		row--;
		if (row >= 0) {
			m_destSushi = m_sushiMatrix[row * m_width + col];
		}
		m_movingVertical = true;
		swapSushi();
		return;
	}

	auto  leftRect = Rect(m_srcSushi->getPositionX() - (halfSushiWidth * 3),
		m_srcSushi->getPositionY() - halfSushiHeight,
		m_srcSushi->getContentSize().width,
		m_srcSushi->getContentSize().height);

	if (leftRect.containsPoint(location)) {
		col--;
		if (col >= 0) {
			m_destSushi = m_sushiMatrix[row * m_width + col];
		}
		m_movingVertical = false;
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
		m_movingVertical = false;
		swapSushi();
		return;
	}

	// not a useful movement
}

void PlayLayer::swapSushi()
{
	m_isAnimationing = true;
	m_isTouchEnable = false;
	if (!m_srcSushi || !m_destSushi) {
		m_movingVertical = true;
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
		|| m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_LINE
		|| m_destSushi->getDisplayMode() == DISPLAY_MODE_5_LINE
		|| ((m_destSushi->getDisplayMode() != DISPLAY_MODE_NORMAL && m_srcSushi->getDisplayMode() != DISPLAY_MODE_NORMAL)
		&& !(m_destSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE && m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE)
		&& !(m_destSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE && m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE)
		)) {
		GameController::getInstance()->onSwapSushiCompleted();
		// just swap
		m_srcSushi->runAction(MoveTo::create(time, posOfDest));
		m_destSushi->runAction(MoveTo::create(time, posOfSrc));
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

	m_srcSushi->runAction(Sequence::create(
		MoveTo::create(time, posOfDest),
		MoveTo::create(time, posOfSrc),
		NULL));
	m_destSushi->runAction(Sequence::create(
		MoveTo::create(time, posOfSrc),
		MoveTo::create(time, posOfDest),
		NULL));
}

// static
bool PlayLayer::isValidRow(int row) {
	return row >= 0 && row < MATRIX_HEIGHT;
}

// static
bool PlayLayer::isValidCol(int col) {
	return col >= 0 && col < MATRIX_WIDTH;
}

void PlayLayer::createAndDropSushi(int row, int col, bool isInit)
{
	Size size = Director::getInstance()->getWinSize();

	int topImgIndex = -1;
	int leftImgIndex = -1;

	if (isInit) {  // if it is an initial round, make the sushis stay un-eliminated status.
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

	SushiSprite *sushi = SushiSprite::create(row, col, topImgIndex, leftImgIndex, PRIORITY_NORMAL);

	// create animation
	Point endPosition = positionOfItem(row, col);
	Point startPosition = Point(endPosition.x, endPosition.y + size.height / 2);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	sushi->setPosition(ccpAdd(visibleOrigin, startPosition));


	if (isInit) 
	{
		float speed = startPosition.y / (1.5 * size.height);
		sushi->runAction(MoveTo::create(speed, endPosition));
	}
	else
	{
		Vector<FiniteTimeAction *> moveVector;
		auto actionMoveTo = MoveTo::create(DROP_SPEED, positionOfItem(MATRIX_HEIGHT - 1, col));

		if (m_moveNumMatrix[row * m_width + col] > 0)
		{
			moveVector.pushBack(DelayTime::create(DROP_SPEED * m_moveNumMatrix[row * m_width + col]));
		}

		moveVector.pushBack(actionMoveTo);
		moveVector.pushBack(MoveTo::create(DROP_SPEED * (MATRIX_HEIGHT - 1 - row), endPosition));
		auto sequence = Sequence::create(moveVector);
		sushi->runAction(sequence);
	}
	m_spriteSheet->addChild(sushi);

	m_sushiMatrix[row * m_width + col] = sushi;
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
		if (m_isNeedFillVacancies) {
			//爆炸效果结束后才掉落新寿司，增强打击感
			fillVacancies();
			m_isNeedFillVacancies = false;
		}
		else {
			checkAndRemoveChain();
		}
	}
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
	//分两种情况 第一种是有src和dist sushi的时候 标记相关生成和移除然后移除     第二种是没有的时候  先标记各种需要移除的情况  然后设置移除    （其中新生成的要设置ignorecheak 同时该设置needremove的也要设置needremove）
	if (m_srcSushi && m_destSushi)
	{
		if (m_destSushi->getDisplayMode() == DISPLAY_MODE_5_LINE && m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_LINE)
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
		else if (m_destSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE && m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE)
		{
			markRemove(m_destSushi);
			markRemove(m_srcSushi);
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE && m_destSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE)
		{
			markRemove(m_destSushi);
			markRemove(m_srcSushi);
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_LINE && (m_destSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE || m_destSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE))
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
					if (sushi->getDisplayMode() == DISPLAY_MODE_NORMAL)
					{
						sushi->setDisplayMode((rand() % 2 == 0) ? DISPLAY_MODE_4_VERTICAL_LINE : DISPLAY_MODE_4_HORIZONTAL_LINE);
						sushi->applyDisplayMode();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_destSushi->getDisplayMode() == DISPLAY_MODE_5_LINE && (m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE || m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE))
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
					if (sushi->getDisplayMode() == DISPLAY_MODE_NORMAL)
					{
						sushi->setDisplayMode((rand() % 2 == 0) ? DISPLAY_MODE_4_VERTICAL_LINE : DISPLAY_MODE_4_HORIZONTAL_LINE);
						sushi->applyDisplayMode();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_LINE && m_destSushi->getDisplayMode() == DISPLAY_MODE_5_CROSS)
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
					if (sushi->getDisplayMode() == DISPLAY_MODE_NORMAL)
					{
						sushi->setDisplayMode(DISPLAY_MODE_5_CROSS);
						sushi->applyDisplayMode();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_destSushi->getDisplayMode() == DISPLAY_MODE_5_LINE && m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_CROSS)
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
					if (sushi->getDisplayMode() == DISPLAY_MODE_NORMAL)
					{
						sushi->setDisplayMode(DISPLAY_MODE_5_CROSS);
						sushi->applyDisplayMode();
					}
					markRemove(sushi);
				}
			}
			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}

		else if (m_destSushi->getDisplayMode() == DISPLAY_MODE_5_CROSS && m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_CROSS)
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

		else if (m_destSushi->getDisplayMode() == DISPLAY_MODE_5_CROSS && (m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE || m_srcSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE) )
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
		else if (m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_CROSS && (m_destSushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE || m_destSushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE))
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
		else if (m_srcSushi->getDisplayMode() == DISPLAY_MODE_5_LINE && m_destSushi->getDisplayMode() != DISPLAY_MODE_5_LINE)
		{
			m_srcSushi->setIsNeedRemove(false);
			markRemove(m_srcSushi);

			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}
		else if (m_destSushi->getDisplayMode() == DISPLAY_MODE_5_LINE && m_srcSushi->getDisplayMode() != DISPLAY_MODE_5_LINE)
		{
			m_destSushi->setIsNeedRemove(false);
			markRemove(m_destSushi);

			m_srcSushi = nullptr;
			m_destSushi = nullptr;
			return;
		}


		std::list<SushiSprite *> colChainList;
		getColChain(m_srcSushi, colChainList);

		std::list<SushiSprite *> rowChainList;
		getRowChain(m_srcSushi, rowChainList);

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

		typedef std::map<SushiSprite*, DisplayMode> SushiModeMap;
		SushiModeMap superSushiModeMap;

		if (colChainList.size() >= 5 || rowChainList.size() >= 5)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setDisplayMode(DISPLAY_MODE_5_LINE);
			m_srcSushi->setIsNeedRemove(false);
			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, DISPLAY_MODE_5_LINE));
		}
		else if (colChainList.size() >= 3 && rowChainList.size() >= 3)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(false);
			m_srcSushi->setDisplayMode(DISPLAY_MODE_5_CROSS);

			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, DISPLAY_MODE_5_CROSS));
		}
		else if (colChainList.size() >= 4)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(false);
			m_srcSushi->setDisplayMode(DISPLAY_MODE_4_HORIZONTAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, DISPLAY_MODE_4_HORIZONTAL_LINE));
		}
		else if (rowChainList.size() >= 4)
		{
			m_srcSushi->setIgnoreCheck(true);
			m_srcSushi->setIsNeedRemove(false);
			m_srcSushi->setDisplayMode(DISPLAY_MODE_4_VERTICAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_srcSushi, DISPLAY_MODE_4_VERTICAL_LINE));
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
			m_destSushi->setDisplayMode(DISPLAY_MODE_5_LINE);
			m_destSushi->setIsNeedRemove(false);
			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, DISPLAY_MODE_5_LINE));
		}
		else if (colChainList.size() >= 3 && rowChainList.size() >= 3)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(false);
			m_destSushi->setDisplayMode(DISPLAY_MODE_5_CROSS);

			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, DISPLAY_MODE_5_CROSS));
		}
		else if (colChainList.size() >= 4)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(false);
			m_destSushi->setDisplayMode(DISPLAY_MODE_4_HORIZONTAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, DISPLAY_MODE_4_HORIZONTAL_LINE));
		}
		else if (rowChainList.size() >= 4)
		{
			m_destSushi->setIgnoreCheck(true);
			m_destSushi->setIsNeedRemove(false);
			m_destSushi->setDisplayMode(DISPLAY_MODE_4_VERTICAL_LINE);

			superSushiModeMap.insert(SushiModeMap::value_type(m_destSushi, DISPLAY_MODE_4_VERTICAL_LINE));
		}

		removeSushi();
		for (SushiModeMap::iterator it = superSushiModeMap.begin(); it != superSushiModeMap.end(); ++it) {
			it->first->applyDisplayMode();
		}

		m_srcSushi = nullptr;
		m_destSushi = nullptr;

	}
	else
	{
		typedef std::map<SushiSprite*, DisplayMode> SushiModeMap;
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
				sushi->setDisplayMode(DISPLAY_MODE_5_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_5_LINE);
				setChainMaxPriority(colChainList, PRIORITY_5_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, DISPLAY_MODE_5_LINE));
			}
			else if (rowChainList.size() >= 5)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setDisplayMode(DISPLAY_MODE_5_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_5_LINE);
				setChainMaxPriority(rowChainList, PRIORITY_5_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, DISPLAY_MODE_5_LINE));
			}
			else if (colChainList.size() >= 3 && rowChainList.size() >= 3 && (int)pRow < (int)PRIORITY_5_CROSS && (int)pCol < (int)PRIORITY_5_CROSS)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setDisplayMode(DISPLAY_MODE_5_CROSS);
				sushi->setSushiPriorityLevel(PRIORITY_5_CROSS);

				setChainMaxPriority(rowChainList, PRIORITY_5_CROSS);
				setChainMaxPriority(colChainList, PRIORITY_5_CROSS);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, DISPLAY_MODE_5_CROSS));
			}
			else if (colChainList.size() >= 4 && (int)pCol < (int)PRIORITY_4_LINE)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setDisplayMode(DISPLAY_MODE_4_HORIZONTAL_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_4_LINE);
				setChainMaxPriority(colChainList, PRIORITY_4_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, DISPLAY_MODE_4_HORIZONTAL_LINE));
			}
			else if (rowChainList.size() >= 4 && (int)pRow < (int)PRIORITY_4_LINE)
			{
				sushi->setIgnoreCheck(true);
				sushi->setIsNeedRemove(false);
				sushi->setDisplayMode(DISPLAY_MODE_4_VERTICAL_LINE);
				sushi->setSushiPriorityLevel(PRIORITY_4_LINE);
				setChainMaxPriority(rowChainList, PRIORITY_4_LINE);

				superSushiModeMap.insert(SushiModeMap::value_type(sushi, DISPLAY_MODE_4_VERTICAL_LINE));
			}
		}
		removeSushi();
		for (SushiModeMap::iterator it = superSushiModeMap.begin(); it != superSushiModeMap.end(); ++it) {
			it->first->applyDisplayMode();
		}
	}
}

bool PlayLayer::checkActualRoundEnd()
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
		if (sushi->getIgnoreCheck()) {
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

void PlayLayer::getColChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList)
{
	if (sushi->getDisplayMode() == DISPLAY_MODE_5_LINE)
		return;

	chainList.push_back(sushi);// add first sushi

	int neighborCol = sushi->getCol() - 1;
	while (neighborCol >= 0) {
		SushiSprite *neighborSushi = m_sushiMatrix[sushi->getRow() * m_width + neighborCol];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex()
			&& neighborSushi->getDisplayMode() != DISPLAY_MODE_5_LINE)) {
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
			&& neighborSushi->getDisplayMode() != DISPLAY_MODE_5_LINE) {
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
			sushiTemp->setDisplayMode(DISPLAY_MODE_NORMAL);
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
	if (sushi->getDisplayMode() == DISPLAY_MODE_5_LINE)
		return;

	chainList.push_back(sushi);// add first sushi

	int neighborRow = sushi->getRow() - 1;
	while (neighborRow >= 0) {
		SushiSprite *neighborSushi = m_sushiMatrix[neighborRow * m_width + sushi->getCol()];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& neighborSushi->getDisplayMode() != DISPLAY_MODE_5_LINE) {
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
			&& neighborSushi->getDisplayMode() != DISPLAY_MODE_5_LINE) {
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
	// make sequence remove
	m_isAnimationing = true;

	int removeCount = 0;

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
			if (sushi->getDisplayMode() == DISPLAY_MODE_5_LINE) {
				explode5LineLineSushi(sushi);
			} else if (sushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE)
			{
				explode4HorizonytalLineSushi(sushi->getPosition());
			} else if (sushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE)
			{
				explode4VerticalLineSushi(sushi->getPosition());
			} else if (sushi->getDisplayMode() == DISPLAY_MODE_5_CROSS) {
				explode5CrossLineSushi(sushi->getPosition());
			}
			explodeSushi(sushi);
			++removeCount;
		}
	}
	if (0 != removeCount)
	{
		GameController::getInstance()->onRemoveSushiCompleted(removeCount);
	}
	else {
		if (isLock())
		{
			m_needRefresh = true;
			refresh();
		}
	}
}

void PlayLayer::explode4HorizonytalLineSushi(Point point) {
	Size size = Director::getInstance()->getWinSize();
	float scaleX = 4;
	float scaleY = 0.7;
	float time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteRight = Sprite::createWithSpriteFrameName(s_colorHRight);
	addChild(colorSpriteRight, 10);
	Point endPosition1 = Point(point.x - size.width, point.y);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	colorSpriteRight->setPosition(startPosition);
	colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
		NULL));

	auto colorSpriteLeft = Sprite::createWithSpriteFrameName(s_colorLRight);
	addChild(colorSpriteLeft, 10);
	Point endPosition2 = Point(point.x + size.width, point.y);
	colorSpriteLeft->setPosition(startPosition);
	colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
		NULL));
}

void PlayLayer::explode4VerticalLineSushi(Point point) {
	Size size = Director::getInstance()->getWinSize();
	float scaleY = 4;
	float scaleX = 0.7;
	float time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteDown = Sprite::createWithSpriteFrameName(s_colorVDown);
	addChild(colorSpriteDown, 10);
	Point endPosition1 = Point(point.x, point.y - size.height);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	colorSpriteDown->setPosition(startPosition);
	colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
		NULL));

	auto colorSpriteUp = Sprite::createWithSpriteFrameName(s_colorVUp);
	addChild(colorSpriteUp, 10);
	Point endPosition2 = Point(point.x, point.y + size.height);
	colorSpriteUp->setPosition(startPosition);
	colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
		NULL));

}

void PlayLayer::explode5LineLineSushi(SushiSprite* sushi) {
}

void PlayLayer::explode5CrossLineSushi(Point point) {
}

void PlayLayer::actionEndCallback(Node *node)
{
	SushiSprite *sushi = (SushiSprite *)node;
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

void PlayLayer::explodeSushi(SushiSprite *sushi)
{
	if (!sushi)
		return;

	float time = 0.3;

	// 1. action for sushi
	sushi->runAction(Sequence::create(
		ScaleTo::create(time, 0.0),
		CallFuncN::create(CC_CALLBACK_1(PlayLayer::actionEndCallback, this)),
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
		CallFunc::create(CC_CALLBACK_0(PlayLayer::didShowScoreNumber, this, label)), nullptr);
	label->runAction(seq);


	// deal with grids
	int row = sushi->getRow();
	int col = sushi->getCol();
	GridSprite* grid = m_gridMatrix[row*m_width + col];
	changeGridType(grid, getGridType(row, col), false);
	
	if (isValidRow(row - 1)) {
		changeGridType(m_gridMatrix[(row - 1)*m_width + col], getGridType(row - 1, col), true);
	}
	if (isValidCol(col + 1)) {
		changeGridType(m_gridMatrix[row*m_width + (col + 1)], getGridType(row, col + 1), true);
	}
	if (isValidRow(row + 1)) {
		changeGridType(m_gridMatrix[(row + 1)*m_width + col], getGridType(row + 1, col), true);
	}
	if (isValidCol(col - 1)) {
		changeGridType(m_gridMatrix[row*m_width + (col - 1)], getGridType(row, col - 1), true);
	}
	
}

void PlayLayer::changeGridType(GridSprite* grid, GridType type, bool isNeighbor) {
	if (!grid)
		return;
	if (isNeighbor) {
		switch (type)
		{
		case GRID_TYPE_CREAM:
			grid->setGridType(GRID_TYPE_DOUBLE_JELLY);
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
			grid->setGridType(GIRD_TYPE_NORMAL);
			break;
		case GRID_TYPE_DOUBLE_JELLY:
			grid->setGridType(GRID_TYPE_JELLY);
			break;
		default:
			break;
		}
	}
	
}

bool PlayLayer::canCreateNewSushi(int index)
{
	//to do 目前设定为 最上面一列的为可掉落
	if (index / m_width == m_height - 1)
	{
		return true;
	}
	return false;
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
bool PlayLayer::bfs(std::deque<int>* sushiStack, int *visited, std::deque<int>* directionStack)  //  0 左上方 1 正上方 2 右上方
{
	if (sushiStack->size() == 0)
	{
		return false;
	}
	if (directionStack->size() == 0)
	{
		return false;
	}

	int index = sushiStack->back();
	int direction = directionStack->back();

	int row = getRowByIndex(index);
	int col = getColByIndex(index);

	row++;
	col = col + direction - 1;
	int curIndex = row * m_width + col;

	if (visited[curIndex])
	{
		if (direction == 2)
		{
			sushiStack->pop_back();
			directionStack->pop_back();
			
			if (directionStack->size() == 0 || sushiStack->size() == 0)
			{
				return false;
			}
			int curDic = directionStack->back();

			if (curDic == 1)
			{
				curDic = 0;
			}
			else if (curDic == 0)
			{
				curDic = 2;
			}
			directionStack->pop_back();
			directionStack->push_back(curDic);

			return bfs(sushiStack, visited, directionStack);
		}
		else if (direction == 0)
		{
			directionStack->pop_back();
			directionStack->push_back(2);
			return bfs(sushiStack, visited, directionStack);
		}
		else if (direction == 1)
		{
			directionStack->pop_back();
			directionStack->push_back(0);
			return bfs(sushiStack, visited, directionStack);
		}
	}

	if (!isValidCol(col) || !isValidRow(row))
	{
		if (direction == 2)
		{
			sushiStack->pop_back();
			directionStack->pop_back();

			if (directionStack->size() == 0 || sushiStack->size() == 0)
			{
				return false;
			}
			int curDic = directionStack->back();

			if (curDic == 1)
			{
				curDic = 0;
			}
			else if (curDic == 0)
			{
				curDic = 2;
			}
			directionStack->pop_back();
			directionStack->push_back(curDic);

			return bfs(sushiStack, visited, directionStack);
		}
		else if (direction == 0)
		{
			directionStack->pop_back();
			directionStack->push_back(2);
			return bfs(sushiStack, visited, directionStack);
		}
		else if (direction == 1)
		{
			directionStack->pop_back();
			directionStack->push_back(0);
			return bfs(sushiStack, visited, directionStack);
		}
	}

	SushiSprite *sushi = m_sushiMatrix[curIndex];
	if (!isValidGrid(row, col)){
		visited[curIndex] = 1;
		if (direction == 2)
		{
			sushiStack->pop_back();
			directionStack->pop_back();

			if (directionStack->size() == 0 || sushiStack->size() == 0)
			{
				return false;
			}
			int curDic = directionStack->back();

			if (curDic == 1)
			{
				curDic = 0;
			}
			else if (curDic == 0)
			{
				curDic = 2;
			}
			directionStack->pop_back();
			directionStack->push_back(curDic);

			return bfs(sushiStack, visited, directionStack);
		}
		else if (direction == 0)
		{
			directionStack->pop_back();
			directionStack->push_back(2);
			return bfs(sushiStack, visited, directionStack);
		}
		else if (direction == 1)
		{
			directionStack->pop_back();
			directionStack->push_back(0);
			return bfs(sushiStack, visited, directionStack);
		}
	}

	if (NULL != sushi)
	{
		sushiStack->push_back(curIndex);
		return true;
	}
	if (canCreateNewSushi(curIndex))
	{
		sushiStack->push_back(curIndex);
		return true;
	}
	if (direction == 2)
	{
		visited[curIndex] = 1;
	}
	sushiStack->push_back(curIndex);
	directionStack->push_back(1);

	return bfs(sushiStack, visited, directionStack);
}

int PlayLayer::getMinEndMove(int row, int col)
{
	int minEndMove = 0;
	if (isValidRow(row - 1) && isValidCol(col) && isValidGrid(row - 1, col) && m_minEndMoveMatrix[(row - 1) * m_width + col] > minEndMove)
	{
		minEndMove = m_minEndMoveMatrix[(row - 1) * m_width + col];
	}
	/*if (isValidRow(row - 1) && isValidCol(col - 1) && isValidGrid(row - 1, col - 1) && m_minEndMoveMatrix[(row - 1) * m_width + col - 1] > minEndMove)
	{
	minEndMove = m_minEndMoveMatrix[(row - 1) * m_width + col - 1];
	}
	if (isValidRow(row - 1) && isValidCol(col + 1) && isValidGrid(row - 1, col + 1) && m_minEndMoveMatrix[(row - 1) * m_width + col + 1] > minEndMove)
	{
	minEndMove = m_minEndMoveMatrix[(row - 1) * m_width + col + 1];
	}*/
	return minEndMove;
}

void PlayLayer::setMoveNum(std::deque<int>* sushiStack, int row, int col)
{
	int minEndMove = getMinEndMove(row, col);
	if (m_moveNumMatrix[sushiStack->back()] + sushiStack->size() - 1 > minEndMove)
	{
		m_minEndMoveMatrix[row * m_width + col] = m_moveNumMatrix[sushiStack->back()] + sushiStack->size() - 1;
	}
	else
	{
		m_minEndMoveMatrix[row * m_width + col] = minEndMove;
		m_moveNumMatrix[sushiStack->back()] = minEndMove - (sushiStack->size() - 1);
	}
}

void PlayLayer::fillVacancies(int row, int col, SushiSprite *sushi)
{

	sushi = m_sushiMatrix[row * m_width + col];
	if (!isValidGrid(row, col)){
		return;
	}
	if (NULL == sushi)
	{
		if (canCreateNewSushi(row * m_width + col))
		{
			createAndDropSushi(row, col, false);
		}
		else
		{
			//此时需要深搜获取掉落路径 深搜结果有三种 1 没有可以用来掉落的路径，此时格子为空 2 有可以用来掉落的路径而且存在可填充格子  3 有可以用来掉落的路径但不存在可填充格子

			std::deque<int>* sushiStack = new std::deque<int>();
			int *visited = new int[m_height * m_width];
			memset(visited, 0, sizeof(int)* m_height * m_width);
			std::deque<int>* directionStack = new std::deque<int>();
			directionStack->push_back(1);
			sushiStack->push_back(row * m_width + col);
			visited[row * m_width + col] = 1;
			bool canDrop = bfs(sushiStack, visited, directionStack);

			if (canDrop)
			{
				SushiSprite *newSushi = NULL;
				newSushi = m_sushiMatrix[sushiStack->back()];
				if (newSushi)
				{
					setMoveNum(sushiStack, row, col);

					//寿司移动
					createAndDropSushi(sushiStack, directionStack, row, col, false);
				}
				else
				{
					//createAndDropSushi(row, col, false);
					//掉落新寿司
					m_moveNumMatrix[sushiStack->back()] = m_moveNumMatrix[sushiStack->back()] + 1;
					setMoveNum(sushiStack, row, col);
					
					createAndDropSushi(sushiStack, directionStack, row, col, true);
				}
			}

			delete sushiStack;
			delete visited;
			delete directionStack;

		}
	}

}

void PlayLayer::fillVacancies()
{
	// reset moving direction flag
	m_movingVertical = true;
	m_isAnimationing = true;

	Size size = CCDirector::getInstance()->getWinSize();

	// 1. drop exist sushi
	SushiSprite *sushi = NULL;

	memset(m_moveNumMatrix, 0, m_width * m_height * sizeof(int));
	memset(m_minEndMoveMatrix, 0, m_width * m_height * sizeof(int));

	for (int row = 0; row < m_height; row++)
	{
		if (m_width % 2 == 0)
		{
			for (int col = 0; col < m_width / 2; col++)
			{
				fillVacancies(row, col, sushi);
				fillVacancies(row, m_width - 1 - col, sushi);
			}
		}
		else
		{
			for (int col = 0; col < m_width / 2; col++)
			{
				fillVacancies(row, col, sushi);
				fillVacancies(row, m_width - 1 - col, sushi);
			}

			fillVacancies(row, m_width / 2, sushi);
		}
	}

	m_needRefresh = false;

	/*bool isActualEnd = checkActualRoundEnd();
	if (isActualEnd)
	{
	GameController::getInstance()->onExplosionStopped();
	}*/
	GameController::getInstance()->onExplosionStopped();

}

void PlayLayer::moveAction(Node *node, std::deque<int>* sushiStack, std::deque<int>* directionStack, int startIndex, bool isCreate)
{
	if (sushiStack->size() > 0)
	{
		SushiSprite *sushi = (SushiSprite *)node;
		Vector<FiniteTimeAction *> moveVector;
		if (m_moveNumMatrix[startIndex] > 0)
		{
			if (isCreate)
			{
				sushi->setVisible(false);
				moveVector.pushBack(DelayTime::create(DROP_SPEED * (m_moveNumMatrix[startIndex])));
				moveVector.pushBack(Show::create());
			}
			else
			{
				moveVector.pushBack(DelayTime::create(DROP_SPEED * (m_moveNumMatrix[startIndex])));
			}
		}
		while (sushiStack->size() > 0)
		{
			int curIndex = sushiStack->back();
			int curCol = getColByIndex(curIndex);
			int curRow = getRowByIndex(curIndex);
			sushiStack->pop_back();

			int dict = directionStack->back();
			directionStack->pop_back();

			auto actionMoveTo = MoveTo::create(DROP_SPEED, positionOfItem(curRow, curCol));
			moveVector.pushBack(actionMoveTo);
			
		}
		auto sequence = Sequence::create(moveVector);
		sushi->runAction(sequence);
	}
}

void PlayLayer::createAndDropSushi(std::deque<int>* sushiStack, std::deque<int>* directionStack, int rowDist, int colDist, bool isCreate)
{
	if (isCreate)
	{
		Size size = Director::getInstance()->getWinSize();

		int topImgIndex = -1;
		int leftImgIndex = -1;

		SushiSprite *sushi = SushiSprite::create(rowDist, colDist, topImgIndex, leftImgIndex, PRIORITY_NORMAL);
		if (m_needRefresh)
		{
			sushi->setDisplayMode((DisplayMode)m_sushiModeMatrix[rowDist * m_width + colDist]);
			sushi->applyDisplayMode();
		}

		// create animation
		Point endPosition = positionOfItem(rowDist, colDist);

		int startIndex = sushiStack->back();

		int row = getRowByIndex(startIndex);
		int col = getColByIndex(startIndex);

		Point startPosition = positionOfItem(row, col);
		Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
		sushi->setPosition(ccpAdd(visibleOrigin, startPosition));
		sushiStack->pop_back();
		moveAction(sushi, sushiStack, directionStack, startIndex, isCreate);
		// add to BatchNode
		m_spriteSheet->addChild(sushi);
		m_sushiMatrix[rowDist * m_width + colDist] = sushi;
	}
	else
	{
		int startIndex = sushiStack->back();
		sushiStack->pop_back();
		int row = getRowByIndex(startIndex);
		int col = getColByIndex(startIndex);

		SushiSprite *sushi = NULL;
		sushi = m_sushiMatrix[row * m_width + col];
		m_sushiMatrix[rowDist * m_width + colDist] = sushi;
		m_sushiMatrix[row * m_width + col] = NULL;
		sushi->setRow(rowDist);
		sushi->setCol(colDist);
		Point startPosition = sushi->getPosition();
		sushi->stopAllActions();// must stop pre drop action
		moveAction(sushi, sushiStack, directionStack, startIndex, isCreate);
	}

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

	if (sushi->getDisplayMode() == DISPLAY_MODE_5_LINE) {  // 所有相同的寿司消除
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
					if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
						tmp->setIsNeedRemove(true);
					}
					else if (tmp->getDisplayMode() != DISPLAY_MODE_NORMAL) {
						markRemove(tmp);
					}
				}
			}
		}
	} else if (sushi->getDisplayMode() == DISPLAY_MODE_4_VERTICAL_LINE) {
		// check for type and mark for certical neighbour
		for (int row = 0; row < m_height; row++) {
			SushiSprite *tmp = m_sushiMatrix[row * m_width + sushi->getCol()];
			if (!tmp || tmp == sushi) {
				continue;
			}

			if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
				tmp->setIsNeedRemove(true);
			}
			else {
				markRemove(tmp);
			}
		}
		
	} else if (sushi->getDisplayMode() == DISPLAY_MODE_4_HORIZONTAL_LINE) {
		// check for type and mark for horizontal neighbour
		for (int col = 0; col < m_width; col++) {
			SushiSprite *tmp = m_sushiMatrix[sushi->getRow() * m_width + col];
			if (!tmp || tmp == sushi) {
				continue;
			}

			if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
				tmp->setIsNeedRemove(true);
			}
			else {
				markRemove(tmp);
			}
		}
	}
	else if (sushi->getDisplayMode() == DISPLAY_MODE_5_CROSS) {  // 九宫格内的寿司消除
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
				if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
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
	GridType type = getGridType(row, col);
	return (GIRD_TYPE_NORMAL == type || GRID_TYPE_JELLY == type || GRID_TYPE_DOUBLE_JELLY == type);
}

GridType PlayLayer::getGridType(int row, int col) {
	if (!isValidRow(row) || !isValidCol(col) || !m_gridMatrix[row*m_width + col])
		return GIRD_TYPE_NONE;
	return m_gridMatrix[row*m_width + col]->getGridType();
}

void PlayLayer::refresh() {
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if (SushiSprite* sushi = m_sushiMatrix[row*m_width + col])
			{
				sushi->setIsNeedRemove(true);
				m_sushiModeMatrix[row*m_width + col] = (int)sushi->getDisplayMode();
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

bool PlayLayer::isLock(int row, int col) {
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
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(topLeftCol1) && isValidCol(topLeftCol2) && isValidRow(topRow1)) {
			sushi1 = m_sushiMatrix[topRow1*m_width + topLeftCol1];
			sushi2 = m_sushiMatrix[topRow1*m_width + topLeftCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(topRightCol1) && isValidCol(topRightCol2) && isValidRow(topRow1)) {
			sushi1 = m_sushiMatrix[topRow1*m_width + topRightCol1];
			sushi2 = m_sushiMatrix[topRow1*m_width + topRightCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
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
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(rightTopRow1) && isValidRow(rightTopRow2) && isValidCol(rightCol1)) {
			sushi1 = m_sushiMatrix[rightTopRow1*m_width + rightCol1];
			sushi2 = m_sushiMatrix[rightTopRow2*m_width + rightCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(rightBottomRow1) && isValidRow(rightBottomRow2) && isValidCol(rightCol1)) {
			sushi1 = m_sushiMatrix[rightBottomRow1*m_width + rightCol1];
			sushi2 = m_sushiMatrix[rightBottomRow2*m_width + rightCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
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
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(bottomLeftCol1) && isValidCol(bottomLeftCol2) && isValidRow(bottomRow1)) {
			sushi1 = m_sushiMatrix[bottomRow1*m_width + bottomLeftCol1];
			sushi2 = m_sushiMatrix[bottomRow1*m_width + bottomLeftCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(bottomRightCol1) && isValidCol(bottomRightCol2) && isValidRow(bottomRow1)) {
			sushi1 = m_sushiMatrix[bottomRow1*m_width + bottomRightCol1];
			sushi2 = m_sushiMatrix[bottomRow1*m_width + bottomRightCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
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
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(leftTopRow1) && isValidRow(leftTopRow2) && isValidCol(leftCol1)) {
			sushi1 = m_sushiMatrix[leftTopRow1*m_width + leftCol1];
			sushi2 = m_sushiMatrix[leftTopRow2*m_width + leftCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(leftBottomRow1) && isValidRow(leftBottomRow2) && isValidCol(leftCol1)) {
			sushi1 = m_sushiMatrix[leftBottomRow1*m_width + leftCol1];
			sushi2 = m_sushiMatrix[leftBottomRow2*m_width + leftCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
	}

	return true;
}

void PlayLayer::didShowScoreNumber(LabelBMFont* label) {
	label->removeFromParent();
	label->release();
}