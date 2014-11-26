#include "View/Layer/TargetLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"

namespace {
const int kLabelTitleTag = 0;
const int kLabelTargetTag = 1;
const int kBackgroundX = 470;
const int kBackgroundY = 320;
const int kLabelTitleX = 380;
const int kLabelTitleY = 320;
const int kLabelTargetX = 470;
const int kLabelTargetY = 320;
}


TargetLayer::TargetLayer()
{
}

TargetLayer::~TargetLayer()
{
}

bool TargetLayer::init()
{
	if (!Layer::init())
		return false;

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(TargetLayer::onRoundEnd),
		MSG_ROUND_END, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(TargetLayer::onRoundReady),
		MSG_ROUND_READY, nullptr);

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("target.png");
	background->setPosition(Vec2(kBackgroundX, kBackgroundY));
	addChild(background);

	// label "Target:"
	auto labelTitle = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
	labelTitle->setPosition(Vec2(kLabelTitleX, kLabelTitleY));
	addChild(labelTitle, 0, kLabelTitleTag);

	// label of target
	auto labelTarget = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelTarget->setPosition(Vec2(kLabelTargetX, kLabelTargetY));
	addChild(labelTarget, 0, kLabelTargetTag);

	// init target
	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	int target = roundInfo.m_targetScroe;
	labelTarget->setString(StringUtils::toString(target));
	return true;
}

void TargetLayer::onRoundEnd(Ref* obj)
{
	
}

void TargetLayer::onRoundReady(Ref* obj)
{
	RoundInfo* roundInfo = (RoundInfo*)obj;
	if (!roundInfo)
		return;
	auto labelTarget = (LabelAtlas*)getChildByTag(kLabelTargetTag);
	int target = roundInfo->m_targetScroe;
	labelTarget->setString(StringUtils::toString(target));
}