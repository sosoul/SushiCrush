#include "View/Scene/PlayScene.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"
#include "View/Layer/PostPlayLayer.h"
#include "View/Layer/PrePlayLayer.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"
#include "View/Layer/TargetTipsLayer.h"
#include "View/Scene/SelectRoundScene.h"
#include "ui/UIButton.h"

PlayScene::PlayScene() : m_prePlayLayer(nullptr),
						 m_postPlayLayer(nullptr),
						 m_playLayer(nullptr),
						 m_targetLayer(nullptr),
						 m_movesLayer(nullptr),
						 m_scoreLayer(nullptr),
						 m_tipsLayer(nullptr)
{
}

PlayScene::~PlayScene()
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

bool PlayScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	// background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto background = Sprite::create(s_mainBackground);
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

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundReady),
		MSG_ROUND_READY, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundEnd),
		MSG_ROUND_END, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onRoundStart),
		MSG_ROUND_START, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onTargetComplete),
		MSG_TARGET_COMPLETE, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onCrushBegin),
		MSG_CRUSH_BEGIN, nullptr);
	// buttons
	auto backButton = ui::Button::create();
	backButton->setTouchEnabled(true);
	backButton->loadTextures(s_backToStartButtonNormal, s_backToStartButtonSelected, "", ui::Button::TextureResType::PLIST);
	backButton->setPosition(Point(visibleSize.width, visibleSize.height) + Point(-70, -50));
	addChild(backButton);
	backButton->addTouchEventListener(this, ui::SEL_TouchEvent(&PlayScene::onBackButtonTouched));

	auto refreshButton = ui::Button::create();
	refreshButton->setTouchEnabled(true);
	refreshButton->loadTextures(s_postPlayResumeNormal, s_postPlayResumeSelected, "", ui::Button::TextureResType::PLIST);
	refreshButton->setPosition(Point(visibleSize.width, visibleSize.height) + Point(-200, -50));
	addChild(refreshButton);
	refreshButton->addTouchEventListener(this, ui::SEL_TouchEvent(&PlayScene::onRefreshButtonTouched));
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

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onTargetComplete),
		MSG_TARGET_COMPLETE, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PlayScene::onCrushBegin),
		MSG_CRUSH_BEGIN, nullptr);

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
	if (!m_postPlayLayer)
		return;
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
	setCascadeColorEnabled(true);
	setColor(Color3B(100, 100, 100));
	showTipsLayer(TIPS_TYPE_TARGET, CallFunc::create(CC_CALLBACK_0(PlayScene::OnTargetTipsActionEnd, this)));

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

void PlayScene::onTargetComplete(Ref* obj) {
	showTipsLayer(TIPS_TYPE_TARGET_COMPLETE, CallFunc::create(CC_CALLBACK_0(PlayScene::onTargetCompleteTipsActionEnd, this, true)));
}

void PlayScene::onCrushBegin(Ref* obj) {
	showTipsLayer(TIPS_TYPE_CRUSH_BEGIN, CallFunc::create(CC_CALLBACK_0(PlayScene::onCrushBeginTipsActionEnd, this, true)));
}

void PlayScene::onRoundEnd(Ref* obj) {
	// play layer - run a hide action.
	if (m_playLayer) {
		m_playLayer->runAction(Sequence::create(MoveBy::create(1, Point(0, 800)), Hide::create(),
			CallFunc::create(CC_CALLBACK_0(PlayScene::onPlayLayerEndActionEnded, this)), nullptr));
	}

	// target layer - run a hide action.
	if (m_targetLayer) {
		m_targetLayer->runAction(Sequence::create(MoveBy::create(1, Point(0, 500)), Hide::create(),
			CallFunc::create(CC_CALLBACK_0(PlayScene::onTargetLayerActionEnded, this)), nullptr));
	}

	bool result = (bool)obj;
	if (result) {
		showTipsLayer(TIPS_TYPE_SUCCESSFUL, CallFunc::create(CC_CALLBACK_0(PlayScene::OnResultTipsActionEnd, this, true)));
	}
	else {
		showTipsLayer(TIPS_TYPE_FAIL, CallFunc::create(CC_CALLBACK_0(PlayScene::OnResultTipsActionEnd, this, false)));
	}
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

void PlayScene::onPlayLayerStartActionEnded() {
	if (m_playLayer)
		m_playLayer->tryPlayGuideAnimation();
}

void PlayScene::onBackButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = SelectRoundScene::create();
		float time = 1.0f;
		CCTransitionScene * transScene = CCTransitionCrossFade::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}

void PlayScene::onRefreshButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		if (m_playLayer)
			m_playLayer->refresh();
	}
}

void PlayScene::onPlayLayerEndActionEnded() {
	m_playLayer->removeFromParentAndCleanup(true);
	m_playLayer->release();
	m_playLayer = nullptr;
}

void PlayScene::onTargetLayerActionEnded() {
	m_targetLayer->removeFromParentAndCleanup(true);
	m_targetLayer->release();
	m_targetLayer = nullptr;
}

void PlayScene::OnTargetTipsActionEnd() {
	setCascadeColorEnabled(true);
	setColor(Color3B(255, 255, 255));

	// play layer - create and run a show action.
	if (!m_playLayer) {
		const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
		m_playLayer = PlayLayer::create(roundInfo.m_round);
		m_playLayer->retain();
		m_playLayer->setCascadeColorEnabled(true);
		addChild(m_playLayer);
	}
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	m_playLayer->setPosition(0, 0 + 500);
	MoveBy* actMoveDown = MoveBy::create(1, Point(0, -500));
	m_playLayer->runAction(Sequence::create(actMoveDown,
		CallFunc::create(CC_CALLBACK_0(PlayScene::onPlayLayerStartActionEnded, this)), nullptr));

	if (!m_targetLayer) {
		m_targetLayer = TargetLayer::create();
		m_targetLayer->retain();
		m_targetLayer->setCascadeColorEnabled(true);
		addChild(m_targetLayer);
	}

	if (m_tipsLayer) {
		m_tipsLayer->removeFromParentAndCleanup(true);
		m_tipsLayer->release();
		m_tipsLayer = nullptr;
	}
}

void PlayScene::OnResultTipsActionEnd(bool result) {
	if (m_tipsLayer) {
		m_tipsLayer->removeFromParentAndCleanup(true);
		m_tipsLayer->release();
		m_tipsLayer = nullptr;
	}
	// post-layer - create and run a show action.
	if (!m_postPlayLayer) {
		m_postPlayLayer = PostPlayLayer::create(result);
		m_postPlayLayer->retain();
		addChild(m_postPlayLayer);
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_postPlayLayer->setPosition(visibleSize.width / 2 - 300, visibleSize.height / 2 - 200);
	MoveBy* movebyAction = MoveBy::create(0.5, Point(200, 0));
	m_postPlayLayer->runAction(Repeat::create(movebyAction, 1));
}

void PlayScene::onTargetCompleteTipsActionEnd(bool result) {
	if (m_tipsLayer) {
		m_tipsLayer->removeFromParentAndCleanup(true);
		m_tipsLayer->release();
		m_tipsLayer = nullptr;
	}
	GameController::getInstance()->changeCurCrushMode();
}

void PlayScene::onCrushBeginTipsActionEnd(bool result) {
	if (m_tipsLayer) {
		m_tipsLayer->removeFromParentAndCleanup(true);
		m_tipsLayer->release();
		m_tipsLayer = nullptr;
	}
	GameController::getInstance()->changeCurCrushMode();
}

void PlayScene::showTipsLayer(TipsLayerType type, CallFunc* func) {
	if (!m_tipsLayer){
		m_tipsLayer = TargetTipsLayer::create(type);
		m_tipsLayer->retain();
		addChild(m_tipsLayer);
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_tipsLayer->setPosition(visibleSize.width / 2 - 300, visibleSize.height / 2);
	auto hideTipsAction = Hide::create();
	auto showTipsAction = Show::create();
	MoveBy* movebyDownAction = MoveBy::create(0.5, Point(0, -500));
	MoveBy* movebyUpAction = MoveBy::create(0.5, Point(0, 500));

	if (func) {
		m_tipsLayer->runAction(Sequence::create(showTipsAction, movebyDownAction, DelayTime::create(1.0),
			movebyUpAction, hideTipsAction, func, nullptr));
	}
}