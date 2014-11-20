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

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(TargetLayer::onRoundChanged),
		MSG_ROUND_CHANGED, nullptr);

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
	onRoundChanged((Ref*)(intptr_t)roundInfo.m_round);
	return true;
}

void TargetLayer::onRoundChanged(Ref* obj)
{
	int round = (int)obj;
	RoundInfo* roundInfo = GameController::getInstance()->getRoundInfo(round);
	if (!roundInfo)
		return;
	int target = roundInfo->m_targetScroe;
	auto labelTarget = (LabelAtlas*)getChildByTag(kLabelTargetTag);
	labelTarget->setString(StringUtils::toString(target));
}