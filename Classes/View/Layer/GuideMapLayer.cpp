#include "View/Layer/GuideMapLayer.h"

#include "Common/Resource.h"
#include "Controller/GameController.h"
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
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background = CCSprite::createWithSpriteFrameName(s_guideMap);
	background->retain();
	background->setAnchorPoint(Vec2::ZERO);
	background->setPosition(origin);
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

	roundButton->loadTextures(normalButton, selectedButton, "", ui::Button::TextureResType::PLIST);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	const int step = 100;
	roundButton->setPosition(ccpAdd(visibleOrigin, Point(round*step, round*step)));
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
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto backButton = ui::Button::create();
	backButton->setTouchEnabled(true);
	backButton->loadTextures(s_backToStartButtonNormal, s_backToStartButtonSelected, "", ui::Button::TextureResType::PLIST);
	backButton->setPosition(Point(visibleSize.width, visibleSize.height) + Point(-70, 0));
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

