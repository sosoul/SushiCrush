#include "View/Layer/PrePlayLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"
#include "ui/UIButton.h"

namespace{
	const int kBackPrePlayX = 100;
	const int kBackYPrePlay = 100;
	const int kStartPlayX = 100;
	const int kStartPlayY = 40;

	const int kNextPlayX = 180;
	const int kNextPlayY = 40;
	const int kLabelTargetTag = 1;
	const int kLabelRoundTag = 5;

	const int kLabelTargetX = 40;
	const int kLabelTargetY = 80;

	const int kLabelRoundX = 40;
	const int kLabelRoundY = 120;

	const int kStar1X = 20;
	const int kStar1Y = 150;
	const int kStar2X = 100;
	const int kStar2Y = 160;
	const int kStar3X = 180;
	const int kStar3Y = 150;

	const int kStart1Tag = 2;
	const int kStart2Tag = 3;
	const int kStart3Tag = 4;
}

PrePlayLayer::PrePlayLayer()
{
}

PrePlayLayer::~PrePlayLayer()
{
}

bool PrePlayLayer::init()
{
	if (!Layer::init())
		return false;

	// background
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_resPlist);
	auto background = CCSprite::createWithSpriteFrameName(s_prePlayBackground);
	background->setPosition(Vec2(visibleOrigin.x + kBackPrePlayX, visibleOrigin.y + kBackYPrePlay));
	addChild(background);

	auto sprite1 = Sprite::create(s_starLeftDone);
	sprite1->setPosition(Vec2(visibleOrigin.x + kStar1X, visibleOrigin.y + kStar1Y));
	addChild(sprite1, 0, kStart1Tag);

	auto sprite2 = Sprite::create(s_starMidDone);
	sprite2->setPosition(Vec2(visibleOrigin.x + kStar2X, visibleOrigin.y + kStar2Y));
	addChild(sprite2, 0, kStart2Tag);

	auto sprite3 = Sprite::create(s_starRightDone);
	sprite3->setPosition(Vec2(visibleOrigin.x + kStar3X, visibleOrigin.y + kStar3Y));
	addChild(sprite3, 0, kStart3Tag);


	ui::Button* startBtn = nullptr;
	startBtn = ui::Button::create(s_preLayerStartNormal, s_preLayerStartSelected, s_preLayerStartSelected);
	startBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PrePlayLayer::start));
	startBtn->setPosition(Vec2(visibleOrigin.x + kStartPlayX, visibleOrigin.y + kStartPlayY));
	addChild(startBtn);


	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	auto labelTarget = LabelAtlas::create("Target:" + StringUtils::toString(roundInfo.m_targetScroe),
		"fonts/tuffy_bold_italic-charmap.plist");
	labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelTarget->setPosition(Vec2(visibleOrigin.x + kLabelTargetX, visibleOrigin.y + kLabelTargetY));
	addChild(labelTarget, 0, kLabelTargetTag);

	auto labelRound = LabelAtlas::create("Round:" + StringUtils::toString(roundInfo.m_round),
		"fonts/tuffy_bold_italic-charmap.plist");
	labelRound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelRound->setPosition(Vec2(visibleOrigin.x + kLabelRoundX, visibleOrigin.y + kLabelRoundY));
	addChild(labelRound, 0, kLabelRoundTag);
	return true;
}

void PrePlayLayer::start(Ref* object, ui::TouchEventType type)
{
	switch (type)
	{
	case ui::TouchEventType::TOUCH_EVENT_ENDED:
	{
		GameController::getInstance()->onRoundStart();
	}
		break;
	default:
		break;
	}
}