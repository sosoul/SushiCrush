#include "View/Scene/SelectRoundScene.h"

#include "View/Layer/SelectRoundLayer.h"


SelectRoundScene::SelectRoundScene()
{
	Scene::init();
}

SelectRoundScene::~SelectRoundScene()
{
}

void SelectRoundScene::onEnter()
{
	Scene::onEnter();
	auto mapLayer = SelectRoundLayer::create();
	auto optionsLayer = GuideMapOptionsLayer::create();
	addChild(mapLayer);
	addChild(optionsLayer);
}