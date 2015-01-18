#include "View/Layer/TargetTipsLayer.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"

namespace {
const int kLabelTargetTag = 0;
const int kBackgroundX = 300;
const int kBackgroundY = 370;
const int kLabelTitleX = 0;
const int kLabelTitleY = 400;
const int kLabelScoreX = 0;
const int kLabelScoreY = 350;
}

TargetTipsLayer::TargetTipsLayer(TipsLayerType type) : _type(type) {
}

TargetTipsLayer::~TargetTipsLayer()
{
}

// static
TargetTipsLayer* TargetTipsLayer::create(TipsLayerType type) {
	TargetTipsLayer* layer = new TargetTipsLayer(type);
	if (layer && layer->init()) {
		layer->autorelease();
		return layer;
	}
	else {
		delete layer;
		layer = nullptr;
		return layer;
	}
}

bool TargetTipsLayer::init() {
	if (!Layer::init())
		return false;
	// background
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::createWithSpriteFrameName(s_targetTipsBackground);
	background->setPosition(Vec2(visibleOrigin.x + kBackgroundX, visibleOrigin.y + kBackgroundY));
	addChild(background);

	switch (_type)
	{
	case TIPS_TYPE_TARGET:
	{
		// label of score
		auto labelTarget = LabelBMFont::create("", "fonts/boundsTestFont.fnt");
		labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		labelTarget->setPosition(Vec2(visibleOrigin.x + kLabelScoreX, visibleOrigin.y + kLabelScoreY));
		addChild(labelTarget, 0, kLabelTargetTag);

		const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
		int score = GameController::getInstance()->getTargetValue(TARGET_TYPE_SCORE);
		int moves = roundInfo.m_totalMoves;
		labelTarget->setString("get at least " + StringUtils::toString(score) + " in " + StringUtils::toString(moves) + " moves");
	}
		break;
	case TIPS_TYPE_SUCCESSFUL:
	{
		auto label = LabelBMFont::create("target", "fonts/boundsTestFont.fnt");
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		label->setPosition(Vec2(visibleOrigin.x + kLabelScoreX, visibleOrigin.y + kLabelScoreY));
		label->setString("congratulations for passing!");
		addChild(label);
	}
		break;
	case TIPS_TYPE_FAIL:
	{
		auto label = LabelBMFont::create("target", "fonts/boundsTestFont.fnt");
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		label->setPosition(Vec2(visibleOrigin.x + kLabelScoreX, visibleOrigin.y + kLabelScoreY));
		label->setString("Failed to pass!");
		addChild(label); }
		break;
	case TIPS_TYPE_TARGET_COMPLATE:
	{
		auto label = LabelBMFont::create("target", "fonts/boundsTestFont.fnt");
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		label->setPosition(Vec2(visibleOrigin.x + kLabelScoreX, visibleOrigin.y + kLabelScoreY));
		label->setString("Target Complate!");
		addChild(label);
	}
		break;
	case TIPS_TYPE_CRASH_BEGIN:
	{
		auto label = LabelBMFont::create("target", "fonts/boundsTestFont.fnt");
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		label->setPosition(Vec2(visibleOrigin.x + kLabelScoreX, visibleOrigin.y + kLabelScoreY));
		label->setString("Crash Time!");
		addChild(label);
	}
		break;
	default:
		break;
	}
	

	return true;
}