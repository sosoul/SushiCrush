#include "View/Layer/MovesLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Types.h"

namespace {
const int kLabelTitleTag = 0;
const int kLabelMovesTag = 1;
const int kBackgroundX = 470;
const int kBackgroundY = 420;
const int kLabelTitleX = 380;
const int kLabelTitleY = 420;
const int kLabelMovesX = 470;
const int kLabelMovesY = 420;
}

MovesLayer::MovesLayer() : label_(NULL)
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
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("moves.png");
	background->setPosition(Vec2(kBackgroundX, kBackgroundY));
	addChild(background);

	// label "Moves:"
	auto labelTitle = LabelBMFont::create("Moves:", "fonts/boundsTestFont.fnt");
	labelTitle->setPosition(Vec2(kLabelTitleX, kLabelTitleY));
	addChild(labelTitle, 0, kLabelTitleTag);

	// label of moves' number
	auto labelMoves = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	labelMoves->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelMoves->setPosition(Vec2(kLabelMovesX, kLabelMovesY));
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