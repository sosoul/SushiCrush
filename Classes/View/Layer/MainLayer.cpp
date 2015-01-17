#include "View/Layer/MainLayer.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"
#include "View/Layer/TargetTipsLayer.h"
#include "View/Scene/GuideMapScene.h"
#include "ui/UIButton.h"

MainLayer::MainLayer():m_playLayer(nullptr),
					   m_targetLayer(nullptr),
					   m_movesLayer(nullptr),
					   m_scoreLayer(nullptr),
					   m_targetTipsLayer(nullptr)
{
}

MainLayer::~MainLayer()
{
	if (m_playLayer) {
		m_playLayer->release();
		m_playLayer = nullptr;
	}
	if (m_targetLayer){
		m_targetLayer->release();
		m_targetLayer = nullptr;
	}
	if (m_movesLayer){
		m_movesLayer->release();
		m_movesLayer = nullptr;
	}
	if (m_scoreLayer){
		m_scoreLayer->release();
		m_scoreLayer = nullptr;
	}
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void MainLayer::onEnter(){
	Layer::onEnter();
}

bool MainLayer::init() {
	if (!Layer::init())
		return false;

	// background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto background = Sprite::createWithSpriteFrameName(s_mainBackground);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	addChild(background);

	m_movesLayer = MovesLayer::create();
	m_movesLayer->retain();
	m_movesLayer->setCascadeColorEnabled(true);
	addChild(m_movesLayer);

	m_scoreLayer = ScoreLayer::create();
	m_scoreLayer->retain();
	m_scoreLayer->setCascadeColorEnabled(true);
	addChild(m_scoreLayer);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MainLayer::onRoundReady),
		MSG_ROUND_READY, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MainLayer::onRoundEnd),
		MSG_ROUND_END, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MainLayer::onRoundStart),
		MSG_ROUND_START, nullptr);

	// buttons
	auto backButton = ui::Button::create();
	backButton->setTouchEnabled(true);
	backButton->loadTextures(s_backToStartButtonNormal, s_backToStartButtonSelected, "", ui::Button::TextureResType::PLIST);
	backButton->setPosition(Point(visibleSize.width, visibleSize.height) + Point(-70, -50));
	addChild(backButton);
	backButton->addTouchEventListener(this, ui::SEL_TouchEvent(&MainLayer::onBackButtonTouched));

	auto refreshButton = ui::Button::create();
	refreshButton->setTouchEnabled(true);
	refreshButton->loadTextures(s_postPlayResumeNormal, s_postPlayResumeSelected, "", ui::Button::TextureResType::PLIST);
	refreshButton->setPosition(Point(visibleSize.width, visibleSize.height) + Point(-200, -50));
	addChild(refreshButton);
	refreshButton->addTouchEventListener(this, ui::SEL_TouchEvent(&MainLayer::onRefreshButtonTouched));
	return true;
}

void MainLayer::onRoundReady(Ref* obj) {
}

void MainLayer::onRoundEnd(Ref* obj) {
	MoveBy* actMoveUp = MoveBy::create(1, Point(0, 500));
	auto hideAction1 = Hide::create();
	// play layer - run a hide action.
	if (m_playLayer) {
		Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
		m_playLayer->setPosition(visibleOrigin.x, visibleOrigin.y);
		m_playLayer->runAction(Sequence::create(actMoveUp, hideAction1,
			CallFunc::create(CC_CALLBACK_0(MainLayer::onPlayLayerActionEnded, this)), nullptr));
	}

	// target layer - run a hide action.
	if (m_targetLayer) {
		m_targetLayer->runAction(Sequence::create(actMoveUp, hideAction1,
			CallFunc::create(CC_CALLBACK_0(MainLayer::onPlayLayerActionEnded, this)), nullptr));
	}
}

void MainLayer::onPlayLayerActionEnded() {
	m_playLayer->removeFromParentAndCleanup(true);
	m_playLayer->release();
	m_playLayer = nullptr;

	m_targetLayer->removeFromParentAndCleanup(true);
	m_targetLayer->release();
	m_targetLayer = nullptr;
}

void MainLayer::onRoundStart(Ref* obj) {
	setCascadeColorEnabled(true);
	setColor(Color3B(100, 100, 100));

	if (!m_targetTipsLayer){
		m_targetTipsLayer = TargetTipsLayer::create();
		m_targetTipsLayer->retain();
		addChild(m_targetTipsLayer);
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_targetTipsLayer->setPosition(visibleSize.width / 2 - 300, visibleSize.height / 2);
	auto hideTipsAction = Hide::create();
	auto showTipsAction = Show::create();
	MoveBy* movebyDownAction = MoveBy::create(0.5, Point(0, -500));
	MoveBy* movebyUpAction = MoveBy::create(0.5, Point(0, 500));
	m_targetTipsLayer->runAction(Sequence::create(showTipsAction, movebyDownAction, DelayTime::create(1.0), movebyUpAction, hideTipsAction, CallFunc::create(CC_CALLBACK_0(MainLayer::onRoundStartActionEnd, this)), nullptr));

}

void MainLayer::onRoundStartActionEnd() {
	setCascadeColorEnabled(true);
	setColor(Color3B(255, 255, 255));

	// play layer - create and run a show action.
	if (!m_playLayer) {
		const CurRoundInfo& roundInfo =  GameController::getInstance()->get_cur_round_info();
		m_playLayer = PlayLayer::create(roundInfo.m_round);
		m_playLayer->retain();
		m_playLayer->setCascadeColorEnabled(true);
		addChild(m_playLayer);
	}
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	m_playLayer->setPosition(0, 0 + 500);
	MoveBy* actMoveDown = MoveBy::create(1, Point(0, -500));
	m_playLayer->runAction(Sequence::create(actMoveDown, nullptr));

	if (!m_targetLayer) {
		m_targetLayer = TargetLayer::create();
		m_targetLayer->retain();
		m_targetLayer->setCascadeColorEnabled(true);
		addChild(m_targetLayer);
	}

	if (m_targetTipsLayer) {
		m_targetTipsLayer->removeFromParentAndCleanup(true);
		m_targetTipsLayer->release();
		m_targetTipsLayer = nullptr;
	}
}

void MainLayer::onBackButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = GuideMapScene::create();
		float time = 1.0f;
		CCTransitionScene * transScene = CCTransitionCrossFade::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}

void MainLayer::onRefreshButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		if (m_playLayer)
			m_playLayer->refresh();
	}
}
