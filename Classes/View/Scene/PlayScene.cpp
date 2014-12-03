#include "View/Scene/PlayScene.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"
#include "View/Layer/MainLayer.h"
#include "View/Layer/PostPlayLayer.h"
#include "View/Layer/PrePlayLayer.h"
#include "View/Layer/TargetTipsLayer.h"

PlayScene::PlayScene() : m_prePlayLayer(nullptr),
						 m_postPlayLayer(nullptr),
						 m_mainLayer(nullptr)
{
}

PlayScene::~PlayScene()
{
	if (m_prePlayLayer) {
		m_prePlayLayer->release();
		m_prePlayLayer = nullptr;
	}
	if (m_postPlayLayer) {
		m_postPlayLayer->release();
		m_postPlayLayer = nullptr;
	}
	if (m_mainLayer){
		m_mainLayer->release();
		m_mainLayer = nullptr;
	}

	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool PlayScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	m_mainLayer = MainLayer::create();
	m_mainLayer->retain();

	addChild(m_mainLayer);
	return true;
}


void PlayScene::onEnter()
{
	Scene::onEnter();

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