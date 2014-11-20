#include "View/Scene/PlayScene.h"

#include "Controller/GameController.h"
#include "View/Layer/MainLayer.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"
#include "Messages.h"
#include "Resource.h"


PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

Scene* PlayScene::createScene() {
	auto scene = Scene::create();
	return scene;
}

void PlayScene::onEnter()
{
	Scene::onEnter();
	auto mainLayer = MainLayer::create();
	auto playLayer = PlayLayer::create();
	auto targetLayer = TargetLayer::create();
	auto movesLayer = MovesLayer::create();
	auto scoreLayer = ScoreLayer::create();
	addChild(mainLayer);
	addChild(playLayer);
	addChild(targetLayer);
	addChild(movesLayer);
	addChild(scoreLayer);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundCompleted),
		MSG_ROUND_COMPLETED, nullptr);
}

void PlayScene::onRoundCompleted(Ref* obj) {
	bool result = (bool)obj;
	Sprite* sprite = NULL;
	if (result)
		sprite = Sprite::create(s_pathLose);
	else
		sprite = Sprite::create(s_pathWin);
	addChild(sprite);
	Size winSize = Director::getInstance()->getWinSize();
	sprite->setPosition(winSize.width / 2, winSize.height / 2);
	auto actionBy = JumpBy::create(2, Vec2(0, 50), 50, 4);
	auto actionByBack = actionBy->reverse();
	sprite->runAction(Sequence::create(actionBy, actionByBack, nullptr));
}