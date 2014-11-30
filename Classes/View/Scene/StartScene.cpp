#include "View/Scene/StartScene.h"

#include "Controller/GameController.h"
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
}