#include "View/Layer/PrePlayLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "ui/UIDeprecated.h"

namespace{
	const int kBackPrePlayX = 100;
	const int kBackYPrePlay = 100;
	const int kResumePlayX = 120;
	const int kResumePlayY = 40;

	const int kNextPlayX = 180;
	const int kNextPlayY = 40;
	const int kLabelTargetTag = 1;
	const int kLabelRoundTag = 5;

	const int kLabelTargetX = 20;
	const int kLabelTargetY = 80;

	const int kLabelRoundX = 20;
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

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PrePlayLayer::onResume),
		MSG_RESUME, nullptr);
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PrePlayLayer::onNextRound),
		MSG_NEXT_ROUND, nullptr);

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("preplay/back.png");
	background->setScaleX(0.5);
	background->setScaleY(0.5);
	background->setPosition(Vec2(kBackPrePlayX, kBackYPrePlay));
	addChild(background);

	auto sprite1 = Sprite::create("preplay/star.png");
	sprite1->setScaleX(0.1);
	sprite1->setScaleY(0.1);
	sprite1->setPosition(Vec2(kStar1X, kStar1Y));
	addChild(sprite1, 0, kStart1Tag);

	auto sprite2 = Sprite::create("preplay/star.png");
	sprite2->setScaleX(0.1);
	sprite2->setScaleY(0.1);
	sprite2->setPosition(Vec2(kStar2X, kStar2Y));
	addChild(sprite2, 0, kStart2Tag);

	auto sprite3 = Sprite::create("preplay/star.png");
	sprite3->setScaleX(0.1);
	sprite3->setScaleY(0.1);
	sprite3->setPosition(Vec2(kStar3X, kStar3Y));
	addChild(sprite3, 0, kStart3Tag);


	ui::Button* resumeBtn = nullptr;
	resumeBtn = ui::Button::create("preplay/resume.png", "preplay/resume.png", "preplay/resume.png", ui::TextureResType::LOCAL);
	resumeBtn->setScale(0.1);
	resumeBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PrePlayLayer::start));
	resumeBtn->setPosition(Vec2(kResumePlayX, kResumePlayY));
	addChild(resumeBtn);

	auto labelTarget = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	labelTarget->setPosition(Vec2(kLabelTargetX, kLabelTargetY));
	addChild(labelTarget, 0, kLabelTargetTag);

	auto labelRound = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
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
		GameController::getInstance()->start();
	}
		break;
	default:
		break;
	}
}


void PrePlayLayer::onResume(Ref* obj)
{
	this->setVisible(true);
	RoundInfo* roundInfo = (RoundInfo*)obj;
	if (!roundInfo)
		return;
	int target = roundInfo->m_targetScroe;
	int round = roundInfo->m_round;
	auto labelTarget = (LabelAtlas*)getChildByTag(kLabelTargetTag);
	labelTarget->setString("target:" + StringUtils::toString(target));
	auto labelRound = (LabelAtlas*)getChildByTag(kLabelRoundTag);
	labelRound->setString("round:" + StringUtils::toString(round));

	auto start1 = (Sprite*)getChildByTag(kStart1Tag);
	start1->setVisible(true);
	auto start2 = (Sprite*)getChildByTag(kStart2Tag);
	start2->setVisible(true);
	auto start3 = (Sprite*)getChildByTag(kStart3Tag);
	start3->setVisible(true);
	int score = roundInfo->m_gotScore;

}

void PrePlayLayer::onNextRound(Ref* obj)
{
	this->setVisible(true);
	RoundInfo* roundInfo = (RoundInfo*)obj;
	if (!roundInfo)
		return;
	int target = roundInfo->m_targetScroe;
	int round = roundInfo->m_round;
	auto labelTarget = (LabelAtlas*)getChildByTag(kLabelTargetTag);
	labelTarget->setString("target:" + StringUtils::toString(target));
	auto labelRound = (LabelAtlas*)getChildByTag(kLabelRoundTag);
	labelRound->setString("round:" + StringUtils::toString(round));

	auto start1 = (Sprite*)getChildByTag(kStart1Tag);
	start1->setVisible(true);
	auto start2 = (Sprite*)getChildByTag(kStart2Tag);
	start2->setVisible(true);
	auto start3 = (Sprite*)getChildByTag(kStart3Tag);
	start3->setVisible(true);
	int score = roundInfo->m_gotScore;

}