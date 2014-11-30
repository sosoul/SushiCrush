#include "View/Layer/MainLayer.h"

#include "Messages.h"
#include "Resource.h"
#include "View/Scene/GuideMapScene.h"
#include "ui/UIButton.h"

MainLayer::MainLayer()
{
}

MainLayer::~MainLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool MainLayer::init() {
	if (!Layer::init())
		return false;

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MainLayer::onRoundReady),
		MSG_ROUND_READY, nullptr);

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(s_mainBackground);
	background->setAnchorPoint(Point(0, 1));
	background->setPosition(Point(0, winSize.height));
	addChild(background);

	// buttons
	auto backButton = ui::Button::create();
	backButton->setTouchEnabled(true);
	backButton->loadTextures(s_backToStartButtonNormal, s_backToStartButtonSelected, "");
	backButton->setPosition(Point(winSize.width, winSize.height) + Point(-50, -50));
	addChild(backButton);
	backButton->addTouchEventListener(this, ui::SEL_TouchEvent(&MainLayer::onBackButtonTouched));
	return true;
}

void MainLayer::onRoundReady(Ref* obj) {
}

void MainLayer::onBackButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = GuideMapScene::create();
		float time = 1.0f;
		CCTransitionScene * transScene = CCTransitionCrossFade::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}
