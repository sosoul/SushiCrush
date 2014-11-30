#include "View/Layer/GuideMapLayer.h"

#include "Controller/GameController.h"
#include "Resource.h"
#include "View/Scene/PlayScene.h"
#include "View/Scene/StartScene.h"
#include "ui/UIButton.h"

GuideMapLayer::GuideMapLayer()
{
}

GuideMapLayer::~GuideMapLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool GuideMapLayer::init()
{
	if (!Layer::init())
		return false;

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = CCSprite::create(s_guideMap);
	background->retain();
	background->setAnchorPoint(Vec2::ZERO);
	background->setPosition(Vec2::ZERO);
	addChild(background);

	// round buttons
	for (int i = 1; i <= 10; ++i)
		createRoundButton(i);

	setTouchEnabled(true);

	return true;
}

void GuideMapLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event) {
}

void GuideMapLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event) {
	if (1 == touches.size()) {
		std::vector<Touch*>::const_iterator it = touches.begin();
		CCPoint delta = (*it)->getDelta();
		this->setPosition(ccpAdd(getPosition(), delta));
	}
}

void GuideMapLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event) {
}

void GuideMapLayer::onRoundButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (!pSender)
		return;
	if (ui::TOUCH_EVENT_ENDED == type) {
		ui::Button* button = static_cast<ui::Button*>(pSender);
		int round = (int)(button->getUserData());
		GameController::getInstance()->setCurRound(round);

		auto scene = PlayScene::create();
		float time = 1.0f;
		CCTransitionScene * transScene = CCTransitionFadeTR::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}

void GuideMapLayer::createRoundButton(int round) {
	auto roundButton = ui::Button::create();
	roundButton->setTouchEnabled(true);

	std::string normalButton = s_roundButtonsNormal[round];
	std::string selectedButton = s_roundButtonsSelected[round];

	roundButton->loadTextures(normalButton, selectedButton, "");
	const int step = 60;
	roundButton->setPosition(Point(round*step, round*step));
	roundButton->setUserData((void*)round);
	addChild(roundButton);
	roundButton->addTouchEventListener(this, ui::SEL_TouchEvent(&GuideMapLayer::onRoundButtonTouched));
}

GuideMapOptionsLayer::GuideMapOptionsLayer() {
}

GuideMapOptionsLayer::~GuideMapOptionsLayer() {
}

bool GuideMapOptionsLayer::init() {
	if (!Layer::init())
		return false;

	// back button
	Size winSize = Director::getInstance()->getWinSize();
	auto backButton = ui::Button::create();
	backButton->setTouchEnabled(true);
	backButton->loadTextures("backToStartNormal.png", "backToStartSelected.png", "");
	backButton->setPosition(Point(winSize.width, winSize.height) + Point(-50, -50));
	addChild(backButton);
	backButton->addTouchEventListener(this, ui::SEL_TouchEvent(&GuideMapOptionsLayer::onBackButtonTouched));
	return true;
}

void GuideMapOptionsLayer::onBackButtonTouched(Ref *pSender, ui::TouchEventType type) {
	if (ui::TOUCH_EVENT_ENDED == type) {
		auto scene = StartScene::create();
		float time = 1.0f;
		CCTransitionScene * transScene = CCTransitionCrossFade::create(time, scene);
		CCDirector::sharedDirector()->replaceScene(transScene);
	}
}

