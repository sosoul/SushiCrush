#include "View/Scene/StartScene.h"

#include "Controller/GameController.h"
#include "View/Layer/MainLayer.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/PlayLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"
#include "Messages.h"
#include "Resource.h"
#include "View/Layer/StartLayer.h"


StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

void StartScene::onEnter()
{
	Scene::onEnter();
	auto layer = StartLayer::create();
	addChild(layer);
}

void StartScene::run() {
	auto layer = StartLayer::create();
	addChild(layer);

	//Director::getInstance()->replaceScene(this);
}