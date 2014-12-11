#include "View/Layer/TargetLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"

namespace {
const int kLabelTitleTag = 0;
const int kLabelTargetTag = 1;
const int kBackgroundX = 120;
const int kBackgroundY = -80;
const int kLabelTitleX = 1;
const int kLabelTitleY = -100;
const int kLabelTargetX = 120;
const int kLabelTargetY = -80;
}


TargetLayer::TargetLayer()
{
}

TargetLayer::~TargetLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
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
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::createWithSpriteFrameName(s_target);
	background->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	background->setPosition(Vec2(visibleOrigin.x + kBackgroundX, visibleOrigin.y + visibleSize.height + kBackgroundY));
	addChild(background);

	// label "Target:"
	auto labelTitle = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
	labelTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelTitle->setPosition(Vec2(visibleOrigin.x + kLabelTitleX, visibleOrigin.y + visibleSize.height + kLabelTitleY));
	addChild(labelTitle, 0, kLabelTitleTag);

	// label of target
	auto labelTarget = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	labelTarget->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelTarget->setPosition(Vec2(visibleOrigin.x + kLabelTargetX, visibleOrigin.y + visibleSize.height + kLabelTargetY));
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