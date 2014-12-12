#include "View/Layer/MovesLayer.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"

namespace {
const int kLabelTitleTag = 0;
const int kLabelMovesTag = 1;
const int kBackgroundX = 120;
const int kBackgroundY = -5;
const int kLabelTitleX = 1;
const int kLabelTitleY = -20;
const int kLabelMovesX = 120;
const int kLabelMovesY = -5;
}

MovesLayer::MovesLayer()
{
}

MovesLayer::~MovesLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool MovesLayer::init() {
	if (!Layer::init())
		return false;

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MovesLayer::onMovesChanged),
		MSG_MOVES_CHANGED, nullptr);
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MovesLayer::onRoundEnd),
		MSG_ROUND_END, nullptr);
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MovesLayer::onRoundReady),
		MSG_ROUND_READY, nullptr);

	// background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::createWithSpriteFrameName(s_moves);
	background->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	background->setPosition(Vec2(visibleOrigin.x + kBackgroundX, visibleOrigin.y + visibleSize.height + kBackgroundY));
	addChild(background);

	// label "Moves:"
	auto labelTitle = LabelBMFont::create("Moves:", "fonts/boundsTestFont.fnt");
	labelTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelTitle->setPosition(Vec2(visibleOrigin.x + kLabelTitleX, visibleOrigin.y + visibleSize.height + kLabelTitleY));
	addChild(labelTitle, 0, kLabelTitleTag);

	// label of moves' number
	auto labelMoves = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	labelMoves->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelMoves->setPosition(Vec2(visibleOrigin.x + kLabelMovesX, visibleOrigin.y + visibleSize.height + kLabelMovesY));
	addChild(labelMoves, 0, kLabelMovesTag);

	// init moves
	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	onMovesChanged((Ref*)(intptr_t)roundInfo.m_totalMoves);
	return true;
}

// notifications
void MovesLayer::onMovesChanged(Ref* obj) {
	int moves = (int)obj;
	int count = getChildrenCount();
	auto labelMoves = (LabelAtlas*)getChildByTag(kLabelMovesTag);
	labelMoves->setString(StringUtils::toString(moves));
}

void MovesLayer::onRoundEnd(Ref* obj) {
	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	int moves = roundInfo.m_totalMoves;
	auto labelMoves = (LabelAtlas*)getChildByTag(kLabelMovesTag);
	labelMoves->setString(StringUtils::toString(moves));
}


void MovesLayer::onRoundReady(Ref* obj)
{
	RoundInfo* roundInfo = (RoundInfo*)obj;
	if (!roundInfo)
		return;
	auto labelMoves = (LabelAtlas*)getChildByTag(kLabelMovesTag);
	int moves = roundInfo->m_totalMoves;
	labelMoves->setString(StringUtils::toString(moves));
}