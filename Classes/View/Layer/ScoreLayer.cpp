#include "View/Layer/ScoreLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"

namespace {
const int kLabelTitleTag = 0;
const int kLabelScoreTag = 1;
const int kBackgroundX = 465;
const int kBackgroundY = 460;
const int kLabelTitleX = 380;
const int kLabelTitleY = 460;
const int kLabelScoreX = 480;
const int kLabelScoreY = 460;
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
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(s_moves);
	background->setPosition(Vec2(kBackgroundX, kBackgroundY));
	addChild(background);

	// label "Score:"
	auto labelTitle = LabelBMFont::create("Score:", "fonts/boundsTestFont.fnt");
	labelTitle->setPosition(Vec2(kLabelTitleX, kLabelTitleY));
	addChild(labelTitle, 0, kLabelTitleTag);

	// label of score
	auto labelScore = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelScore->setPosition(Vec2(kLabelScoreX, kLabelScoreY));
	addChild(labelScore, 0, kLabelScoreTag);

	// init score
	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	onScoreChanged((Ref*)(intptr_t)roundInfo.m_gotScore);
	return true;
}

void ScoreLayer::onScoreChanged(Ref* obj) {
	int score = (int)obj;
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