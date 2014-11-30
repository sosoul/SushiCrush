#include "View/Layer/StartLayer.h"

#include "Resource.h"
#include "ui/UIButton.h"
#include "View/Scene/GuideMapScene.h"

StartLayer::StartLayer()
{
}

StartLayer::~StartLayer()
{
}

bool StartLayer::init()
{
	if (!Layer::init())
		return false;

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(s_startBackground);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(Point(winSize.width/2, winSize.height/2));
	addChild(background);

	// buttons
	auto startButton = ui::Button::create();
	startButton->setTouchEnabled(true);
	startButton->loadTextures(s_startNormal, s_startSelected, "");
	startButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(0, -50));
	addChild(startButton);
	startButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onStartButtonTouched));

	auto musicButton = ui::Button::create();
	musicButton->setTouchEnabled(true);
	musicButton->loadTextures(s_musicNormal, s_musicSelected, "");
	musicButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(-100, -150));
	addChild(musicButton);
	musicButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	auto soundButton = ui::Button::create();
	soundButton->setTouchEnabled(true);
	soundButton->loadTextures(s_soundNormal, s_soundSelected, "");
	soundButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(0, -150));
	addChild(soundButton);
	soundButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	auto rateButton = ui::Button::create();
	rateButton->setTouchEnabled(true);
	rateButton->loadTextures(s_rateNormal, s_rateSelected, "");
	rateButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(100, -150));
	addChild(rateButton);
	rateButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	return true;
}

void StartLayer::onStartButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = GuideMapScene::create();
		float time = 1.0f;
		CCTransitionScene * transScene = CCTransitionFadeTR::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}

void StartLayer::onMusicButtonTouched(Ref *pSender, ui::TouchEventType type) {
}

void StartLayer::onSoundButtonTouched(Ref *pSender, ui::TouchEventType type) {
}

void StartLayer::onRateButtonTouched(Ref *pSender, ui::TouchEventType type) {
}