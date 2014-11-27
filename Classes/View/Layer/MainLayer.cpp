#include "View/Layer/MainLayer.h"

#include "Messages.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"
#include "Resource.h"
#include "View/Scene/StartScene.h"
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
	auto background = Sprite::create("mainBackground.jpg");
	background->setAnchorPoint(Point(0, 1));
	background->setPosition(Point(0, winSize.height));
	addChild(background);

	// buttons
	auto backToStartButton = ui::Button::create();
	backToStartButton->setTouchEnabled(true);
	backToStartButton->loadTextures("backToStartNormal.png", "backToStartSelected.png", "");
	backToStartButton->setPosition(Point(winSize.width, winSize.height) + Point(-50, -50));
	addChild(backToStartButton);
	backToStartButton->addTouchEventListener(this, ui::SEL_TouchEvent(&MainLayer::onBackToStartButtonTouched));
	return true;
}

void MainLayer::onRoundReady(Ref* obj) {
}

void MainLayer::onBackToStartButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = StartScene::create();
		float time = 2.0f;
		CCTransitionScene * transScene = CCTransitionPageTurn::create(time, scene, false);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}
