#include "View/Scene/GuideMapScene.h"

#include "View/Layer/GuideMapLayer.h"


GuideMapScene::GuideMapScene()
{
	Scene::init();
}

GuideMapScene::~GuideMapScene()
{
}

void GuideMapScene::onEnter()
{
	Scene::onEnter();
	auto mapLayer = GuideMapLayer::create();
	auto optionsLayer = GuideMapOptionsLayer::create();
	addChild(mapLayer);
	addChild(optionsLayer);
}