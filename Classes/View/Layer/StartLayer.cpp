#include "View/Layer/StartLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"
#include "View/Scene/PlayScene.h"

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
	auto background = Sprite::create("startBackground.png");
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(Point(winSize.width/2, winSize.height/2));
	addChild(background);

	// buttons
	auto startButton = ui::Button::create();
	startButton->setTouchEnabled(true);
	startButton->loadTextures("startNormal.png", "startSelected.png", "");
	startButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(0, -50));
	addChild(startButton);
	startButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onStartButtonTouched));

	auto musicButton = ui::Button::create();
	musicButton->setTouchEnabled(true);
	musicButton->loadTextures("musicNormal.png", "musicSelected.png", "");
	musicButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(-100, -150));
	addChild(musicButton);
	musicButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	auto soundButton = ui::Button::create();
	soundButton->setTouchEnabled(true);
	soundButton->loadTextures("soundNormal.png", "soundSelected.png", "");
	soundButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(0, -150));
	addChild(soundButton);
	soundButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	auto rateButton = ui::Button::create();
	rateButton->setTouchEnabled(true);
	rateButton->loadTextures("rateNormal.png", "rateSelected.png", "");
	rateButton->setPosition(Point(winSize.width / 2, winSize.height / 2) + Point(100, -150));
	addChild(rateButton);
	rateButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	return true;
}

void StartLayer::onStartButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = PlayScene::create();
		float time = 2.0f;
		CCTransitionScene * transScene = CCTransitionSlideInT::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}

void StartLayer::onMusicButtonTouched(Ref *pSender, ui::TouchEventType type) {
}

void StartLayer::onSoundButtonTouched(Ref *pSender, ui::TouchEventType type) {
}

void StartLayer::onRateButtonTouched(Ref *pSender, ui::TouchEventType type) {
}