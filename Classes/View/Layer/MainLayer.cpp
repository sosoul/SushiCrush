#include "View/Layer/MainLayer.h"

#include "Messages.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"
#include "Resource.h"

MainLayer::MainLayer()
{
}

MainLayer::~MainLayer()
{
}

bool MainLayer::init() {
	if (!Layer::init())
		return false;

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MainLayer::onRoundChanged),
		MSG_ROUND_CHANGED, nullptr);

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("mainBackground.jpg");
	background->setAnchorPoint(Point(0, 1));
	background->setPosition(Point(0, winSize.height));
	addChild(background);
}

void MainLayer::onRoundChanged(Ref* obj) {
}
