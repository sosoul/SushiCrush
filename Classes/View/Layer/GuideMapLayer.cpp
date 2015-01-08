#include "View/Layer/GuideMapLayer.h"

#include "Common/Resource.h"
#include "Controller/GameController.h"
#include "View/Scene/PlayScene.h"
#include "View/Scene/StartScene.h"
#include "ui/UIButton.h"
#include "Common/TiledMapParser.h"

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
	std::string filePath = FileUtils::getInstance()->fullPathForFilename("guideMap.tmx");
	TiledMapParser* tiledMapParser = TiledMapParser::create(filePath);
	TMXLayer* layer = tiledMapParser->createLayer("backgroundLayer");
	addChild(layer);

	// round buttons
	MapGidToGamePos map;
	tiledMapParser->getGidToGamePosMap("roundIconLayer", &map);
	MapGidToGamePos::iterator it = map.begin();
	for (; map.end() != it; ++it) {
		int round = it->first - 1;
		createRoundButton(round, GameController::getInstance()->isUnlock(round), it->second);
	}

	setTouchEnabled(true);

	return true;
}

void GuideMapLayer::createRoundButton(int round, bool isUnlock, const Point& pos) {
	auto roundButton = ui::Button::create();

	std::string normalButton;
	std::string selectedButton = s_roundButtonsSelected[round];
	if (isUnlock) {
		normalButton = s_roundButtonsNormal[round];
	}
	else {
		normalButton = selectedButton;
	}
	roundButton->loadTextures(normalButton, selectedButton, "", ui::Button::TextureResType::PLIST);
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	const int step = 100;
	roundButton->setPosition(ccpAdd(visibleOrigin, pos));
	roundButton->setUserData((void*)round);
	addChild(roundButton);
	roundButton->addTouchEventListener(this, ui::SEL_TouchEvent(&GuideMapLayer::onRoundButtonTouched));
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

