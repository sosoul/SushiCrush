#include "View/Layer/PlayLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"
#include "View/Sprite/SushiSprite.h"

namespace {
// TODO: The matrix data need to be read from config file.

const int kRoundMatrixes[10][8][7] = { { { 0, 0, 0, 1, 0, 0, 0 },
										 { 0, 0, 0, 1, 0, 0, 0 },
										 { 0, 0, 0, 1, 0, 0, 0 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 0, 0, 0, 1, 0, 0, 0 },
										 { 0, 0, 0, 1, 0, 0, 0 },
										 { 0, 0, 0, 1, 0, 0, 0 },
										 { 0, 0, 0, 1, 0, 0, 0 } }, // 1

									   { { 0, 0, 0, 0, 0, 0, 0 },
										 { 0, 0, 1, 1, 1, 0, 0 },
										 { 0, 1, 1, 1, 1, 1, 0 },
										 { 0, 1, 1, 1, 1, 1, 0 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 },
										 { 1, 1, 1, 1, 1, 1, 1 } },  // 2

									  { { 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 0 },
										{ 1, 1, 1, 1, 1, 0, 0 },
										{ 1, 1, 1, 1, 0, 0, 0 },
										{ 1, 1, 1, 0, 0, 0, 0 } },  // 3

									  { { 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 0, 1, 1, 1, 0, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 1, 1, 1, 1 },
										{ 1, 1, 1, 0, 1, 1, 1 },
										{ 1, 1, 0, 0, 0, 1, 1 },
										{ 1, 1, 0, 0, 0, 0, 1 },
										{ 1, 0, 0, 0, 0, 0, 1 } },  // 4

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
						 m_matrix(NULL),
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
						 m_round(round)
{
}

PlayLayer::~PlayLayer()
{
	if (m_matrix) {
		free(m_matrix);
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
	m_matrix = (SushiSprite **)malloc(arraySize);
	memset((void*)m_matrix, 0, arraySize);

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
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if (hasSushi(row, col))
				createAndDropSushi(row, col, true);
		}
	}
}

SushiSprite *PlayLayer::sushiOfPoint(Point *point)
{
	SushiSprite *sushi = NULL;
	Rect rect = Rect(0, 0, 0, 0);

	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_matrix[i];
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
			m_destSushi = m_matrix[row * m_width + col];
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
			m_destSushi = m_matrix[row * m_width + col];
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
			m_destSushi = m_matrix[row * m_width + col];
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
			m_destSushi = m_matrix[row * m_width + col];
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
	m_matrix[m_srcSushi->getRow() * m_width + m_srcSushi->getCol()] = m_destSushi;
	m_matrix[m_destSushi->getRow() * m_width + m_destSushi->getCol()] = m_srcSushi;
	int tmpRow = m_srcSushi->getRow();
	int tmpCol = m_srcSushi->getCol();
	m_srcSushi->setRow(m_destSushi->getRow());
	m_srcSushi->setCol(m_destSushi->getCol());
	m_destSushi->setRow(tmpRow);
	m_destSushi->setCol(tmpCol);

	// 2.check for remove able
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
		|| rowChainListOfSecond.size() >= 3) {
		GameController::getInstance()->onSwapSushiCompleted();
		// just swap
		m_srcSushi->runAction(MoveTo::create(time, posOfDest));
		m_destSushi->runAction(MoveTo::create(time, posOfSrc));
		return;
	}

	// 3.no chain, swap back
	m_matrix[m_srcSushi->getRow() * m_width + m_srcSushi->getCol()] = m_destSushi;
	m_matrix[m_destSushi->getRow() * m_width + m_destSushi->getCol()] = m_srcSushi;
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
			SushiSprite* topSushi1 = m_matrix[topRow1*m_width + col];
			SushiSprite* topSushi2 = m_matrix[topRow2*m_width + col];
			if (topSushi1 && topSushi2 && topSushi1->getImgIndex() == topSushi2->getImgIndex())
				topImgIndex = topSushi1->getImgIndex();
		}

		// scan the sushis on the left
		int leftCol1 = col - 1;
		int leftCol2 = col - 2;
		if (isValidCol(leftCol1) && isValidCol(leftCol2)) {
			SushiSprite* leftSushi1 = m_matrix[row*m_width + leftCol1];
			SushiSprite* leftSushi2 = m_matrix[row*m_width + leftCol2];
			if (leftSushi1 && leftSushi2 && leftSushi1->getImgIndex() == leftSushi2->getImgIndex())
				leftImgIndex = leftSushi1->getImgIndex();
		}
	}

	SushiSprite *sushi = SushiSprite::create(row, col, topImgIndex, leftImgIndex);

	// create animation
	Point endPosition = positionOfItem(row, col);
	Point startPosition = Point(endPosition.x, endPosition.y + size.height / 2);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	sushi->setPosition(ccpAdd(visibleOrigin, startPosition));
	float speed = startPosition.y / (1.5 * size.height);
	sushi->runAction(MoveTo::create(speed, endPosition));
	// add to BatchNode
	m_spriteSheet->addChild(sushi);

	m_matrix[row * m_width + col] = sushi;
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
			SushiSprite *sushi = m_matrix[i];
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
		sushi = m_matrix[i];
		if (!sushi) {
			continue;
		}
		sushi->setIgnoreCheck(false);
	}

	// 2. check chain
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_matrix[i];
		if (!sushi) {
			continue;
		}

		if (sushi->getIsNeedRemove()) {
			continue;// 已标记过的跳过检查
		}
		if (sushi->getIgnoreCheck()) {
			continue;// 新变化的特殊寿司，不消除
		}

		// start count chain
		std::list<SushiSprite *> colChainList;
		getColChain(sushi, colChainList);

		std::list<SushiSprite *> rowChainList;
		getRowChain(sushi, rowChainList);

		std::list<SushiSprite *> &longerList = colChainList.size() > rowChainList.size() ? colChainList : rowChainList;
		if (longerList.size() < 3) {
			continue;// 小于3个不消除
		}

		std::list<SushiSprite *>::iterator itList;
		bool isSetedIgnoreCheck = false;
		for (itList = longerList.begin(); itList != longerList.end(); itList++) {
			sushi = (SushiSprite *)*itList;
			if (!sushi) {
				continue;
			}

			if (longerList.size() > 3) {
				// 4消产生特殊寿司
				if (sushi == m_srcSushi || sushi == m_destSushi) {
					isSetedIgnoreCheck = true;
					sushi->setIgnoreCheck(true);
					sushi->setIsNeedRemove(false);
					sushi->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
					continue;
				}
			}

			markRemove(sushi);
		}

		// 如何是自由掉落产生的4消, 取最后一个变化为特殊寿司
		if (!isSetedIgnoreCheck && longerList.size() > 3) {
			sushi->setIgnoreCheck(true);
			sushi->setIsNeedRemove(false);
			sushi->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
		}
	}

	// 3.消除标记了的寿司
	removeSushi();
}

bool PlayLayer::checkActualRoundEnd()
{
	SushiSprite *sushi;
	// 1. reset ingnore flag
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_matrix[i];
		if (!sushi) {
			continue;
		}
		sushi->setIgnoreCheck(false);
	}

	// 2. check chain
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_matrix[i];
		if (!sushi) {
			continue;
		}

		if (sushi->getIsNeedRemove()) {
			continue;// 已标记过的跳过检查
		}
		if (sushi->getIgnoreCheck()) {
			continue;// 新变化的特殊寿司，不消除
		}

		// start count chain
		std::list<SushiSprite *> colChainList;
		getColChain(sushi, colChainList);

		std::list<SushiSprite *> rowChainList;
		getRowChain(sushi, rowChainList);

		std::list<SushiSprite *> &longerList = colChainList.size() > rowChainList.size() ? colChainList : rowChainList;
		if (longerList.size() < 3) {
			continue;// 小于3个不消除
		}

		std::list<SushiSprite *>::iterator itList;
		bool isSetedIgnoreCheck = false;
		for (itList = longerList.begin(); itList != longerList.end(); itList++) {
			sushi = (SushiSprite *)*itList;
			if (!sushi) {
				continue;
			}

			if (longerList.size() > 3) {
				// 4消产生特殊寿司
				if (sushi == m_srcSushi || sushi == m_destSushi) {
					isSetedIgnoreCheck = true;
					sushi->setIgnoreCheck(true);
					sushi->setIsNeedRemove(false);
					sushi->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
					continue;
				}
			}

			markRemove(sushi);
		}

		// 如何是自由掉落产生的4消, 取最后一个变化为特殊寿司
		if (!isSetedIgnoreCheck && longerList.size() > 3) {
			sushi->setIgnoreCheck(true);
			sushi->setIsNeedRemove(false);
			sushi->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
		}
	}

	int removeCount = 0;

	for (int i = 0; i < m_height * m_width; i++) {
		SushiSprite *sushi = m_matrix[i];
		if (!sushi) {
			continue;
		}

		if (sushi->getIsNeedRemove()) {
			++removeCount;
		}
	}
	if (0 != removeCount)
	{
		return false;
	}
	return true;
}


void PlayLayer::getColChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList)
{
	chainList.push_back(sushi);// add first sushi

	int neighborCol = sushi->getCol() - 1;
	while (neighborCol >= 0) {
		SushiSprite *neighborSushi = m_matrix[sushi->getRow() * m_width + neighborCol];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& !neighborSushi->getIsNeedRemove()
			&& !neighborSushi->getIgnoreCheck()) {
			chainList.push_back(neighborSushi);
			neighborCol--;
		}
		else {
			break;
		}
	}

	neighborCol = sushi->getCol() + 1;
	while (neighborCol < m_width) {
		SushiSprite *neighborSushi = m_matrix[sushi->getRow() * m_width + neighborCol];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& !neighborSushi->getIsNeedRemove()
			&& !neighborSushi->getIgnoreCheck()) {
			chainList.push_back(neighborSushi);
			neighborCol++;
		}
		else {
			break;
		}
	}
}

void PlayLayer::getRowChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList)
{
	chainList.push_back(sushi);// add first sushi

	int neighborRow = sushi->getRow() - 1;
	while (neighborRow >= 0) {
		SushiSprite *neighborSushi = m_matrix[neighborRow * m_width + sushi->getCol()];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& !neighborSushi->getIsNeedRemove()
			&& !neighborSushi->getIgnoreCheck()) {
			chainList.push_back(neighborSushi);
			neighborRow--;
		}
		else {
			break;
		}
	}

	neighborRow = sushi->getRow() + 1;
	while (neighborRow < m_height) {
		SushiSprite *neighborSushi = m_matrix[neighborRow * m_width + sushi->getCol()];
		if (neighborSushi
			&& (neighborSushi->getImgIndex() == sushi->getImgIndex())
			&& !neighborSushi->getIsNeedRemove()
			&& !neighborSushi->getIgnoreCheck()) {
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
		SushiSprite *sushi = m_matrix[i];
		if (!sushi) {
			continue;
		}

		if (sushi->getIsNeedRemove()) {
			m_isNeedFillVacancies = true;  // 需要掉落
			// TODO: 检查类型，并播放一行消失的动画

			if (sushi->getDisplayMode() == DISPLAY_MODE_HORIZONTAL)
			{
				explodeSpecialH(sushi->getPosition());
			}
			else if (sushi->getDisplayMode() == DISPLAY_MODE_VERTICAL)
			{
				explodeSpecialV(sushi->getPosition());
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
			refresh();
	}
}

void PlayLayer::explodeSpecialH(Point point)
{
	Size size = Director::getInstance()->getWinSize();
	float scaleX = 4;
	float scaleY = 0.7;
	float time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteRight = Sprite::create(s_colorHRight);
	addChild(colorSpriteRight, 10);
	Point endPosition1 = Point(point.x - size.width, point.y);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	colorSpriteRight->setPosition(ccpAdd(visibleOrigin, startPosition));
	colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
		NULL));

	auto colorSpriteLeft = Sprite::create(s_colorLRight);
	addChild(colorSpriteLeft, 10);
	Point endPosition2 = Point(point.x + size.width, point.y);
	colorSpriteLeft->setPosition(ccpAdd(visibleOrigin, startPosition));
	colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
		NULL));


}

void PlayLayer::explodeSpecialV(Point point)
{
	Size size = Director::getInstance()->getWinSize();
	float scaleY = 4;
	float scaleX = 0.7;
	float time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteDown = Sprite::create(s_colorVDown);
	addChild(colorSpriteDown, 10);
	Point endPosition1 = Point(point.x, point.y - size.height);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	colorSpriteDown->setPosition(ccpAdd(visibleOrigin, startPosition));
	colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
		NULL));

	auto colorSpriteUp = Sprite::create(s_colorVUp);
	addChild(colorSpriteUp, 10);
	Point endPosition2 = Point(point.x, point.y + size.height);
	colorSpriteUp->setPosition(ccpAdd(visibleOrigin, startPosition));
	colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
		NULL));
}

void PlayLayer::actionEndCallback(Node *node)
{
	SushiSprite *sushi = (SushiSprite *)node;
	m_matrix[sushi->getRow() * m_width + sushi->getCol()] = NULL;
	sushi->removeFromParent();
}

void PlayLayer::explodeSushi(SushiSprite *sushi)
{
	float time = 0.3;

	// 1. action for sushi
	sushi->runAction(Sequence::create(
		ScaleTo::create(time, 0.0),
		CallFuncN::create(CC_CALLBACK_1(PlayLayer::actionEndCallback, this)),
		NULL));

	// 2. action for circle
	auto circleSprite = Sprite::create(s_circle);
	addChild(circleSprite, 10);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	circleSprite->setPosition(ccpAdd(visibleOrigin, sushi->getPosition()));
	circleSprite->setScale(0);// start size
	circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
		NULL));

	// 3. particle effect
	auto particleStars = ParticleSystemQuad::create(s_stars);
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(ccpAdd(visibleOrigin, sushi->getPosition()));
	particleStars->setScale(0.3);
	addChild(particleStars, 20);
}

void PlayLayer::fillVacancies()
{
	// reset moving direction flag
	m_movingVertical = true;
	m_isAnimationing = true;

	Size size = CCDirector::getInstance()->getWinSize();
	int *colEmptyInfo = (int *)malloc(sizeof(int) * m_width);
	memset((void *)colEmptyInfo, 0, sizeof(int) * m_width);

	// 1. drop exist sushi
	SushiSprite *sushi = NULL;
	for (int col = 0; col < m_width; col++) {
		int removedSushiOfCol = 0;
		// from buttom to top
		for (int row = 0; row < m_height; row++) {
			sushi = m_matrix[row * m_width + col];
			if (NULL == sushi && hasSushi(row, col)) {
				++removedSushiOfCol;
			} else {
				if (!sushi)
					continue;
				if (removedSushiOfCol > 0) {
					int newRow = row - removedSushiOfCol;
					if (!hasSushi(newRow, col) && m_matrix[newRow*m_width + col])
						continue;
					// switch in matrix
					m_matrix[newRow * m_width + col] = sushi;
					m_matrix[row * m_width + col] = NULL;
					// move to new position
					Point startPosition = sushi->getPosition();
					Point endPosition = positionOfItem(newRow, col);
					float speed = (startPosition.y - endPosition.y) / size.height;
					sushi->stopAllActions();// must stop pre drop action
					sushi->runAction(CCMoveTo::create(speed, endPosition));
					// set the new row to item
					sushi->setRow(newRow);
				}
			}
		}

		// record empty info
		colEmptyInfo[col] = removedSushiOfCol;
	}

	// 2. create new item and drop down.
	for (int col = 0; col < m_width; col++) {
		int topInValidRowsCount = getTopInValidRowsCount(col);
		for (int row = m_height - colEmptyInfo[col] - topInValidRowsCount; row < m_height; row++) {
			if (hasSushi(row, col))
				createAndDropSushi(row, col, false);
		}
	}

	free(colEmptyInfo);

	bool isActualEnd = checkActualRoundEnd();
	if (isActualEnd)
	{
		GameController::getInstance()->onExplosionStopped();
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
	// check for type and mark for certical neighbour
	if (sushi->getDisplayMode() == DISPLAY_MODE_VERTICAL) {
		for (int row = 0; row < m_height; row++) {
			SushiSprite *tmp = m_matrix[row * m_width + sushi->getCol()];
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
		// check for type and mark for horizontal neighbour
	}
	else if (sushi->getDisplayMode() == DISPLAY_MODE_HORIZONTAL) {
		for (int col = 0; col < m_width; col++) {
			SushiSprite *tmp = m_matrix[sushi->getRow() * m_width + col];
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

void PlayLayer::onRoundEnd(Ref* obj) {

}

void PlayLayer::onRoundStart(Ref* obj) {
	return;
	SushiSprite *sushi;
	// 1. reset ingnore flag
	for (int i = 0; i < m_height * m_width; i++) {
		sushi = m_matrix[i];
		if (!sushi) {
			continue;
		}
		sushi->setIsNeedRemove(true);
		markRemove(sushi);
	}
}

bool PlayLayer::hasSushi(int row, int col) {
	if (m_round <= 0 || m_round > TOTAL_ROUND ||
		row < 0 || row >= m_height ||
		col < 0 || col >= m_width)
		return false;
	if (kRoundMatrixes[m_round-1][row][col])
		return true;
	return false;
}

int PlayLayer::getTopInValidRowsCount(int col) {
	int result = 0;
	if (m_round <= 0 || m_round > TOTAL_ROUND ||
		col < 0 || col >= m_width)
		return result;
	for (int i = m_height - 1; i >= 0; --i) {
		if (!kRoundMatrixes[m_round - 1][i][col])
			++result;
		else
			break;
	}
	return result;
}

void PlayLayer::refresh() {
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			if (SushiSprite* sushi = m_matrix[row*m_width + col])
				sushi->setIsNeedRemove(true);
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
	if (!hasSushi(row, col))
		return true;
	if (!isValidRow(row) || !isValidCol(col))
		return false;
	SushiSprite* sushi = m_matrix[row*m_width+col];
	if (!sushi)
		return false;
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
	if (hasSushi(topRow1, col)) {
		if (isValidRow(topRow2) && isValidRow(topRow3)) {
			sushi1 = m_matrix[topRow2*m_width + col];
			sushi2 = m_matrix[topRow3*m_width + col];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(topLeftCol1) && isValidCol(topLeftCol2) && isValidRow(topRow1)) {
			sushi1 = m_matrix[topRow1*m_width + topLeftCol1];
			sushi2 = m_matrix[topRow1*m_width + topLeftCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(topRightCol1) && isValidCol(topRightCol2) && isValidRow(topRow1)) {
			sushi1 = m_matrix[topRow1*m_width + topRightCol1];
			sushi2 = m_matrix[topRow1*m_width + topRightCol2];
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
	if (hasSushi(row, rightCol1)) {
		if (isValidCol(rightCol2) && isValidCol(rightCol3)) {
			sushi1 = m_matrix[row*m_width + rightCol2];
			sushi2 = m_matrix[row*m_width + rightCol3];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(rightTopRow1) && isValidRow(rightTopRow2) && isValidCol(rightCol1)) {
			sushi1 = m_matrix[rightTopRow1*m_width + rightCol1];
			sushi2 = m_matrix[rightTopRow2*m_width + rightCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(rightBottomRow1) && isValidRow(rightBottomRow2) && isValidCol(rightCol1)) {
			sushi1 = m_matrix[rightBottomRow1*m_width + rightCol1];
			sushi2 = m_matrix[rightBottomRow2*m_width + rightCol1];
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
	if (hasSushi(bottomRow1, col)) {
		if (isValidRow(bottomRow2) && isValidRow(bottomRow3)) {
			sushi1 = m_matrix[bottomRow2*m_width + col];
			sushi2 = m_matrix[bottomRow3*m_width + col];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(bottomLeftCol1) && isValidCol(bottomLeftCol2) && isValidRow(bottomRow1)) {
			sushi1 = m_matrix[bottomRow1*m_width + bottomLeftCol1];
			sushi2 = m_matrix[bottomRow1*m_width + bottomLeftCol2];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidCol(bottomRightCol1) && isValidCol(bottomRightCol2) && isValidRow(bottomRow1)) {
			sushi1 = m_matrix[bottomRow1*m_width + bottomRightCol1];
			sushi2 = m_matrix[bottomRow1*m_width + bottomRightCol2];
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
	if (hasSushi(row, leftCol1)) {
		if (isValidCol(leftCol2) && isValidCol(leftCol3)) {
			sushi1 = m_matrix[row*m_width + leftCol2];
			sushi2 = m_matrix[row*m_width + leftCol3];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(leftTopRow1) && isValidRow(leftTopRow2) && isValidCol(leftCol1)) {
			sushi1 = m_matrix[leftTopRow1*m_width + leftCol1];
			sushi2 = m_matrix[leftTopRow2*m_width + leftCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
		if (isValidRow(leftBottomRow1) && isValidRow(leftBottomRow2) && isValidCol(leftCol1)) {
			sushi1 = m_matrix[leftBottomRow1*m_width + leftCol1];
			sushi2 = m_matrix[leftBottomRow2*m_width + leftCol1];
			if (canbeRemovedSushis(sushi1, sushi2, index))
				return false;
		}
	}

	return true;
}