#include "View/Layer/PrePlayLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"
#include "ui/UIButton.h"

namespace{
	const int kBackPrePlayX = 100;
	const int kBackYPrePlay = 100;
	const int kResumePlayX = 120;
	const int kResumePlayY = 40;

	const int kNextPlayX = 180;
	const int kNextPlayY = 40;
	const int kLabelTargetTag = 1;
	const int kLabelRoundTag = 5;

	const int kLabelTargetX = 0;
	const int kLabelTargetY = 80;

	const int kLabelRoundX = 0;
	const int kLabelRoundY = 120;

	const int kStar1X = 20;
	const int kStar1Y = 160;
	const int kStar2X = 60;
	const int kStar2Y = 160;
	const int kStar3X = 100;
	const int kStar3Y = 160;

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
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(s_prePlayBackground);
	background->setScaleX(0.5);
	background->setScaleY(0.5);
	background->setPosition(Vec2(kBackPrePlayX, kBackYPrePlay));
	addChild(background);

	auto sprite1 = Sprite::create(s_prePlayStar);
	sprite1->setScaleX(0.1);
	sprite1->setScaleY(0.1);
	sprite1->setPosition(Vec2(kStar1X, kStar1Y));
	addChild(sprite1, 0, kStart1Tag);

	auto sprite2 = Sprite::create(s_prePlayStar);
	sprite2->setScaleX(0.1);
	sprite2->setScaleY(0.1);
	sprite2->setPosition(Vec2(kStar2X, kStar2Y));
	addChild(sprite2, 0, kStart2Tag);

	auto sprite3 = Sprite::create(s_prePlayStar);
	sprite3->setScaleX(0.1);
	sprite3->setScaleY(0.1);
	sprite3->setPosition(Vec2(kStar3X, kStar3Y));
	addChild(sprite3, 0, kStart3Tag);


	ui::Button* resumeBtn = nullptr;
	resumeBtn = ui::Button::create(s_prePlayResume, s_prePlayResume, s_prePlayResume);
	resumeBtn->setScale(0.1);
	resumeBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PrePlayLayer::start));
	resumeBtn->setPosition(Vec2(kResumePlayX, kResumePlayY));
	addChild(resumeBtn);


	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	auto labelTarget = LabelAtlas::create("Target score:" + StringUtils::toString(roundInfo.m_targetScroe),
		"fonts/tuffy_bold_italic-charmap.plist");
	labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	labelTarget->setPosition(Vec2(kLabelTargetX, kLabelTargetY));
	addChild(labelTarget, 0, kLabelTargetTag);

	auto labelRound = LabelAtlas::create("round:" + StringUtils::toString(roundInfo.m_round),
		"fonts/tuffy_bold_italic-charmap.plist");
	labelRound->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	labelRound->setPosition(Vec2(kLabelRoundX, kLabelRoundY));
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