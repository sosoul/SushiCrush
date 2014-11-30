#include "View/Scene/PlayScene.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"
#include "View/Layer/MainLayer.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"

namespace{
	const int kPlayLayerTag = 1;
}

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
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
	addChild(playLayer, 0, kPlayLayerTag);
	addChild(targetLayer);
	addChild(movesLayer);
	addChild(scoreLayer);
	
	Size winSize = Director::getInstance()->getWinSize();

	m_postPlayLayer = PostPlayLayer::create();
	m_postPlayLayer->setPosition(winSize.width / 2 - 100, winSize.height / 2 - 200);
	addChild(m_postPlayLayer);
	m_postPlayLayer->setVisible(false);

	m_prePlayLayer = PrePlayLayer::create();
	m_prePlayLayer->setPosition(winSize.width / 2 - 100, winSize.height / 2 - 200);
	addChild(m_prePlayLayer);
	m_prePlayLayer->setVisible(false);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundEnd),
		MSG_ROUND_END, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundReady),
		MSG_ROUND_READY, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundStart),
		MSG_ROUND_START, nullptr);

	playLayer->setVisible(false);
	GameController::getInstance()->onRoundReady(ACTION_RESUME);
}

void PlayScene::onRoundEnd(Ref* obj) {
	bool result = (bool)obj;
	Sprite* sprite = NULL;
	if (result)
		sprite = Sprite::create(s_pathWin);
	else
		sprite = Sprite::create(s_pathLose);
	addChild(sprite);
	Size winSize = Director::getInstance()->getWinSize();
	sprite->setPosition(winSize.width / 2, winSize.height / 2);
	auto actionBy = JumpBy::create(1, Vec2(0, 50), 50, 2);
	auto actionByBack = actionBy->reverse();
	auto hideAction = Hide::create();
	sprite->runAction(Sequence::create(actionBy, actionByBack, hideAction, nullptr));

	
	m_prePlayLayer->setVisible(false);

	m_postPlayLayer->setPosition(winSize.width / 2 - 300, winSize.height / 2 - 200);
	m_postPlayLayer->setVisible(true);
	MoveBy* act1 = MoveBy::create(0.5, Point(200, 0));
	m_postPlayLayer->runAction(Repeat::create(act1, 1));

	auto playLayer = (PlayLayer*)getChildByTag(kPlayLayerTag);
	MoveBy* actMoveUp = MoveBy::create(1, Point(0, 500));
	auto hideAction1 = Hide::create();
	playLayer->setPosition(0, 0);
	playLayer->runAction(Sequence::create(actMoveUp, hideAction1, nullptr));
}

void PlayScene::onRoundReady(Ref* obj) {

	Size winSize = Director::getInstance()->getWinSize();
	m_prePlayLayer->setPosition(winSize.width / 2 - 500, winSize.height / 2 - 200);

	auto hideAction = Hide::create();
	MoveBy* actMoveRight = MoveBy::create(0.5, Point(400, 0));

	auto preSeq = Sequence::create(
		actMoveRight,
		hideAction,
		nullptr);
	m_prePlayLayer->setVisible(true);
	MoveBy* actMoveRight2 = MoveBy::create(0.5, Point(400, 0));
	m_prePlayLayer->runAction(Repeat::create(actMoveRight2, 1));
	m_postPlayLayer->runAction(preSeq);
}

void PlayScene::onRoundStart(Ref* obj) {
	m_prePlayLayer->setVisible(false);

	auto playLayer = (PlayLayer*)getChildByTag(kPlayLayerTag);
	playLayer->setPosition(0, 500);
	playLayer->setVisible(true);
	MoveBy* actMoveDown = MoveBy::create(1, Point(0, -500));
	playLayer->runAction(Repeat::create(actMoveDown, 1));
}