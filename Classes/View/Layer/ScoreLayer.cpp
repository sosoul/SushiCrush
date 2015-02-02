#include "View/Layer/ScoreLayer.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"

namespace {
const int kLabelTitleTag = 0;
const int kLabelScoreTag = 1;
const int kBackgroundX = 400;
const int kBackgroundY = -5;
const int kLabelTitleX = 290;
const int kLabelTitleY = -20;
const int kLabelScoreX = 400;
const int kLabelScoreY = -5;
}

ScoreLayer::ScoreLayer()
{
}

ScoreLayer::~ScoreLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool ScoreLayer::init() {
	if (!Layer::init())
		return false;
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ScoreLayer::onRoundEnd),
		MSG_ROUND_END, nullptr);
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ScoreLayer::onRoundReady),
		MSG_ROUND_READY, nullptr);
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ScoreLayer::onScoreChanged),
		MSG_SCORE_CHANGED, nullptr);

	// background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::createWithSpriteFrameName(s_moves);
	background->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	background->setPosition(Vec2(visibleOrigin.x + kBackgroundX, visibleOrigin.y + visibleSize.height + kBackgroundY));
	addChild(background);

	// label "Score:"
	auto labelTitle = LabelBMFont::create("Score:", "fonts/boundsTestFont.fnt");
	labelTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelTitle->setPosition(Vec2(visibleOrigin.x + kLabelTitleX, visibleOrigin.y + visibleSize.height + kLabelTitleY));
	addChild(labelTitle, 0, kLabelTitleTag);

	// label of score
	auto labelScore = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	labelScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelScore->setPosition(Vec2(visibleOrigin.x + kLabelScoreX, visibleOrigin.y + visibleSize.height + kLabelScoreY));
	addChild(labelScore, 0, kLabelScoreTag);

	// init score
	const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	int gotScore = GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SCORE);
	onScoreChanged((Ref*)(intptr_t)gotScore);
	return true;
}

void ScoreLayer::onScoreChanged(Ref* obj) {
	int score = *(int*)obj;
	int count = getChildrenCount();
	auto labelScore = (LabelAtlas*)getChildByTag(kLabelScoreTag);
	labelScore->setString(StringUtils::toString(score));
}

void ScoreLayer::onRoundEnd(Ref* obj) {
	
}

void ScoreLayer::onRoundReady(Ref* obj)
{
	auto labelScore = (LabelAtlas*)getChildByTag(kLabelScoreTag);
	labelScore->setString(StringUtils::toString(0));
}