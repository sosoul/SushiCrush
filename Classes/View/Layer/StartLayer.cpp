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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// background
	auto background = Sprite::createWithSpriteFrameName(s_startBackground);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	addChild(background);

	// buttons
	auto startButton = ui::Button::create();
	startButton->setTouchEnabled(true);
	startButton->loadTextures(s_startNormal, s_startSelected, "", ui::Button::TextureResType::PLIST);
	startButton->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2) + Point(0, -100));
	addChild(startButton);
	startButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onStartButtonTouched));

	auto musicButton = ui::Button::create();
	musicButton->setTouchEnabled(true);
	musicButton->loadTextures(s_musicNormal, s_musicSelected, "", ui::Button::TextureResType::PLIST);
	musicButton->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2) + Point(-200, -250));
	addChild(musicButton);
	musicButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	auto soundButton = ui::Button::create();
	soundButton->setTouchEnabled(true);
	soundButton->loadTextures(s_soundNormal, s_soundSelected, "", ui::Button::TextureResType::PLIST);
	soundButton->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2) + Point(0, -250));
	addChild(soundButton);
	soundButton->addTouchEventListener(this, ui::SEL_TouchEvent(&StartLayer::onMusicButtonTouched));

	auto rateButton = ui::Button::create();
	rateButton->setTouchEnabled(true);
	rateButton->loadTextures(s_rateNormal, s_rateSelected, "", ui::Button::TextureResType::PLIST);
	rateButton->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2) + Point(200, -250));
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