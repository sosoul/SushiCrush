#include "View/Scene/PlayScene.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"
#include "View/Layer/MainLayer.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/PostPlayLayer.h"
#include "View/Layer/PrePlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"

PlayScene::PlayScene() : m_playLayer(nullptr),
						 m_prePlayLayer(nullptr),
						 m_postPlayLayer(nullptr)
{
}

PlayScene::~PlayScene()
{
	if (m_playLayer) {
		m_playLayer->release();
		m_playLayer = nullptr;
	}
	if (m_prePlayLayer) {
		m_prePlayLayer->release();
		m_prePlayLayer = nullptr;
	}
	if (m_postPlayLayer) {
		m_postPlayLayer->release();
		m_postPlayLayer = nullptr;
	}
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void PlayScene::onEnter()
{
	Scene::onEnter();
	auto mainLayer = MainLayer::create();
	auto targetLayer = TargetLayer::create();
	auto movesLayer = MovesLayer::create();
	auto scoreLayer = ScoreLayer::create();
	addChild(mainLayer);
	addChild(targetLayer);
	addChild(movesLayer);
	addChild(scoreLayer);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundEnd),
		MSG_ROUND_END, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundReady),
		MSG_ROUND_READY, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundStart),
		MSG_ROUND_START, nullptr);

	GameController::getInstance()->onRoundReady(ACTION_RESUME);
}

void PlayScene::onRoundReady(Ref* obj) {
	Size winSize = Director::getInstance()->getWinSize();

	// pre-play layer - create and run a show action.
	if (!m_prePlayLayer) {
		m_prePlayLayer = PrePlayLayer::create();
		m_prePlayLayer->retain();
		addChild(m_prePlayLayer);
	}
	m_prePlayLayer->setPosition(winSize.width / 2 - 500, winSize.height / 2 - 200);
	MoveBy* movebyAction = MoveBy::create(0.5, Point(400, 0));
	m_prePlayLayer->runAction(Repeat::create(movebyAction, 1));

	// post-play layer  - run a hide action.
	RoundInfo* roundInfo = static_cast<RoundInfo*>(obj);
	if (1 == roundInfo->m_round)
		return;
	if (!m_postPlayLayer) {
		m_postPlayLayer = PostPlayLayer::create();
		m_postPlayLayer->retain();
		addChild(m_postPlayLayer);
	}
	auto hideAction = Hide::create();
	movebyAction = MoveBy::create(0.5, Point(400, 0));
	auto sequence = Sequence::create(
		movebyAction,
		hideAction,
		CallFunc::create(CC_CALLBACK_0(PlayScene::onPostPlayLayerActionEnded, this)),
		nullptr);
	m_postPlayLayer->runAction(sequence);
}

void PlayScene::onRoundStart(Ref* obj) {
	if (!obj)
		return;
	RoundInfo* roundInfo = static_cast<RoundInfo*>(obj);
	// pre-play layer - run a hide action.
	auto hideAction = Hide::create();
	MoveBy* movebyAction = MoveBy::create(0.5, Point(400, 0));
	auto sequence = Sequence::create(
		movebyAction,
		hideAction,
		CallFunc::create(CC_CALLBACK_0(PlayScene::onPrePlayLayerActionEnded, this)),
		nullptr);
	m_prePlayLayer->runAction(sequence);

	// play layer - create and run a show action.
	if (!m_playLayer) {
		m_playLayer = PlayLayer::create(roundInfo->m_round);
		m_playLayer->retain();
		addChild(m_playLayer);
	}
	m_playLayer->setPosition(0, 500);
	MoveBy* actMoveDown = MoveBy::create(1, Point(0, -500));
	m_playLayer->runAction(Repeat::create(actMoveDown, 1));
	
}

void PlayScene::onRoundEnd(Ref* obj) {
	Size winSize = Director::getInstance()->getWinSize();

	// post-layer - create and run a show action.
	if (!m_postPlayLayer) {
		m_postPlayLayer = PostPlayLayer::create();
		m_postPlayLayer->retain();
		addChild(m_postPlayLayer);
	}
	m_postPlayLayer->setPosition(winSize.width / 2 - 300, winSize.height / 2 - 200);
	MoveBy* movebyAction = MoveBy::create(0.5, Point(200, 0));
	m_postPlayLayer->runAction(Repeat::create(movebyAction, 1));

	// play layer - run a hide action.
	if (m_playLayer) {
		MoveBy* actMoveUp = MoveBy::create(1, Point(0, 500));
		auto hideAction1 = Hide::create();
		m_playLayer->setPosition(0, 0);
		m_playLayer->runAction(Sequence::create(actMoveUp, hideAction1,
			CallFunc::create(CC_CALLBACK_0(PlayScene::onPlayLayerActionEnded, this)), nullptr));
	}
}

void PlayScene::onPlayLayerActionEnded() {
	m_playLayer->removeFromParentAndCleanup(true);
	m_playLayer->release();
	m_playLayer = nullptr;
}

void PlayScene::onPostPlayLayerActionEnded() {
	m_postPlayLayer->removeFromParentAndCleanup(true);
	m_postPlayLayer->release();
	m_postPlayLayer = nullptr;
}

void PlayScene::onPrePlayLayerActionEnded() {
	m_prePlayLayer->removeFromParentAndCleanup(true);
	m_prePlayLayer->release();
	m_prePlayLayer = nullptr;
}