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
	auto actionBy = JumpBy::create(2, Vec2(0, 50), 50, 4);
	auto actionByBack = actionBy->reverse();
	sprite->runAction(Sequence::create(actionBy, actionByBack, nullptr));

	m_postPlayLayer->setVisible(true);
	m_prePlayLayer->setVisible(false);
}

void PlayScene::onRoundReady(Ref* obj) {
	m_postPlayLayer->setVisible(false);
	m_prePlayLayer->setVisible(true);
}

void PlayScene::onRoundStart(Ref* obj) {
	m_prePlayLayer->setVisible(false);
}