#include "View/Layer/PostPlayLayer.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"
#include "View/Scene/SelectRoundScene.h"
#include "ui/UIButton.h"

namespace{
	const int kBackPrePlayX = 100;
	const int kBackYPrePlay = 100;

	const int kResumePlayX = 100;
	const int kResumePlayY = 40;

	const int kListPlayX = 40;
	const int kListPlayY = 40;

	const int kNextPlayX = 160;
	const int kNextPlayY = 40;
	const int kLabelTargetTag = 1;

	const int kLabelTargetX = 20;
	const int kLabelTargetY = 80;

	const int kStar1X = 20;
	const int kStar1Y = 150;
	const int kStar2X = 100;
	const int kStar2Y = 160;
	const int kStar3X = 180;
	const int kStar3Y = 150;
}

PostPlayLayer* PostPlayLayer::create(bool result) {
	PostPlayLayer* layer = new PostPlayLayer(result);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	else {
		delete layer;
		layer = nullptr;
		return layer;
	}
}

PostPlayLayer::PostPlayLayer(bool result) : _result(result)
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
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto background = CCSprite::createWithSpriteFrameName(s_postPlayBackground);
	background->setPosition(Vec2(visibleOrigin.x + kBackPrePlayX, visibleOrigin.y + kBackYPrePlay));
	addChild(background);

	ui::Button* resumeBtn = nullptr;
	resumeBtn = ui::Button::create(s_postPlayResumeNormal, s_postPlayResumeSelected, s_postPlayResumeSelected, ui::Button::TextureResType::PLIST);
	resumeBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PostPlayLayer::resume));
	resumeBtn->setPosition(Vec2(visibleOrigin.x + kResumePlayX, visibleOrigin.y +kResumePlayY));
	addChild(resumeBtn);

	ui::Button* nextBtn = nullptr;
	nextBtn = ui::Button::create(s_postPlayNextNormal, s_postPlayNextSelected, s_postPlayNextSelected, ui::Button::TextureResType::PLIST);
	nextBtn->setScale(1);
	nextBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PostPlayLayer::nextRound));
	nextBtn->setPosition(Vec2(visibleOrigin.x + kNextPlayX, visibleOrigin.y +kNextPlayY));
	addChild(nextBtn);

	ui::Button* listBtn = nullptr;
	listBtn = ui::Button::create(s_postPlayListNormal, s_postPlayListSelected, s_postPlayListSelected, ui::Button::TextureResType::PLIST);
	listBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&PostPlayLayer::backToStartMap));
	listBtn->setPosition(Vec2(visibleOrigin.x + kListPlayX, visibleOrigin.y + kListPlayY));
	addChild(listBtn);

	const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	auto labelTarget = LabelAtlas::create("round:" + StringUtils::toString(roundInfo.m_round),
		"fonts/tuffy_bold_italic-charmap.plist");
	labelTarget->setPosition(Vec2(visibleOrigin.x + kLabelTargetX, visibleOrigin.y + kLabelTargetY));
	addChild(labelTarget);

	int gotScore = GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SCORE);
	int targetScore = GameController::getInstance()->getTargetValue(TARGET_TYPE_SCORE);
	if (nextBtn) {
		if (_result)
			nextBtn->setVisible(true);
		else
			nextBtn->setVisible(false);
	}
	

	if (gotScore >= targetScore)
	{
		auto sprite1 = Sprite::createWithSpriteFrameName(s_starLeftDone);
		sprite1->setPosition(Vec2(visibleOrigin.x + kStar1X, visibleOrigin.y + kStar1Y));
		addChild(sprite1);
	}
	else
	{
		auto sprite1 = Sprite::createWithSpriteFrameName(s_starLeftUndone);
		sprite1->setPosition(Vec2(visibleOrigin.x + kStar1X, visibleOrigin.y +  kStar1Y));
		addChild(sprite1);
	}
	if (gotScore >= targetScore * 2)
	{
		auto sprite2 = Sprite::createWithSpriteFrameName(s_starMidDone);
		sprite2->setPosition(Vec2(visibleOrigin.x + kStar2X, visibleOrigin.y + kStar2Y));
		addChild(sprite2);
	}
	else
	{
		auto sprite2 = Sprite::createWithSpriteFrameName(s_starMidUndone);
		sprite2->setPosition(Vec2(visibleOrigin.x + kStar2X, visibleOrigin.y + kStar2Y));
		addChild(sprite2);
	}
	if (gotScore >= targetScore * 3)
	{
		auto sprite3 = Sprite::createWithSpriteFrameName(s_starRightDone);
		sprite3->setPosition(Vec2(visibleOrigin.x + kStar3X, visibleOrigin.y + kStar3Y));
		addChild(sprite3);
	}
	else
	{
		auto sprite3 = Sprite::createWithSpriteFrameName(s_starRightUndone);
		sprite3->setPosition(Vec2(visibleOrigin.x + kStar3X, visibleOrigin.y + kStar3Y));
		addChild(sprite3);
	}
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

void PostPlayLayer::backToStartMap(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = SelectRoundScene::create();
		float time = 1.0f;
		CCTransitionScene * transScene = CCTransitionCrossFade::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}