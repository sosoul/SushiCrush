#include "View/Layer/PostPlayLayer.h"

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

	const int kLabelTargetX = 20;
	const int kLabelTargetY = 80;

	const int kStar1X = 20;
	const int kStar1Y = 120;
	const int kStar2X = 60;
	const int kStar2Y = 120;
	const int kStar3X = 100;
	const int kStar3Y = 120;

	const int kStart1Tag = 2;
	const int kStart2Tag = 3;
	const int kStart3Tag = 4;
}

PostPlayLayer::PostPlayLayer()
{
}

PostPlayLayer::~PostPlayLayer()
{
}

bool PostPlayLayer::init()
{
	if (!Layer::init())
		return false;

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(s_postPlayBackground);
	background->setScaleX(0.5);
	background->setScaleY(0.5);
	background->setPosition(Vec2(kBackPrePlayX, kBackYPrePlay));
	addChild(background);

	auto sprite1 = Sprite::create(s_postPlaySatr);
	sprite1->setScaleX(0.1);
	sprite1->setScaleY(0.1);
	sprite1->setPosition(Vec2(kStar1X, kStar1Y));
	addChild(sprite1, 0, kStart1Tag);

	auto sprite2 = Sprite::create(s_postPlaySatr);
	sprite2->setScaleX(0.1);
	sprite2->setScaleY(0.1);
	sprite2->setPosition(Vec2(kStar2X, kStar2Y));
	addChild(sprite2, 0, kStart2Tag);

	auto sprite3 = Sprite::create(s_postPlaySatr);
	sprite3->setScaleX(0.1);
	sprite3->setScaleY(0.1);
	sprite3->setPosition(Vec2(kStar3X, kStar3Y));
	addChild(sprite3, 0, kStart3Tag);


	ui::Button* resumeBtn = nullptr;
	resumeBtn = ui::Button::create(s_postPlayResume, s_postPlayResume, s_postPlayResume);
	resumeBtn->setScale(0.1);
	resumeBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PostPlayLayer::resume));
	resumeBtn->setPosition(Vec2(kResumePlayX, kResumePlayY));
	addChild(resumeBtn);

	ui::Button* nextBtn = nullptr;
	nextBtn = ui::Button::create(s_postPlayNext, s_postPlayNext, s_postPlayNext);
	nextBtn->setScale(1);
	nextBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PostPlayLayer::nextRound));
	nextBtn->setPosition(Vec2(kNextPlayX, kNextPlayY));
	addChild(nextBtn);

	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	auto labelTarget = LabelAtlas::create("round:" + StringUtils::toString(roundInfo.m_round),
		"fonts/tuffy_bold_italic-charmap.plist");
	labelTarget->setPosition(Vec2(kLabelTargetX, kLabelTargetY));
	addChild(labelTarget, 0, kLabelTargetTag);

	int target = roundInfo.m_targetScroe;
	int score = roundInfo.m_gotScore;

	if (score >= target)
		sprite1->setVisible(true);
	if (score >= target * 2)
		sprite2->setVisible(true);
	if (score >= target * 3)
		sprite3->setVisible(true);
	return true;
}

void PostPlayLayer::resume(Ref* object, ui::TouchEventType type)
{
	switch (type)
	{
		case ui::TouchEventType::TOUCH_EVENT_ENDED:
		{
			GameController::getInstance()->onRoundReady(ACTION_RESUME);
		}
			break;
		default:
			break;
	}
}

void PostPlayLayer::nextRound(Ref* object, ui::TouchEventType type)
{
	switch (type)
	{
		case ui::TouchEventType::TOUCH_EVENT_ENDED:
		{
			GameController::getInstance()->onRoundReady(ACTION_NEXT_ROUND);
		}
			break;
		default:
			break;
	}
}