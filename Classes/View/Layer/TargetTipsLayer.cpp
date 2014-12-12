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

TargetTipsLayer::TargetTipsLayer()
{
}

TargetTipsLayer::~TargetTipsLayer()
{
}

bool TargetTipsLayer::init() {
	if (!Layer::init())
		return false;
	// background
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::createWithSpriteFrameName(s_targetTipsBackground);
	background->setPosition(Vec2(visibleOrigin.x + kBackgroundX, visibleOrigin.y + kBackgroundY));
	addChild(background);
	// label "Score:"
	auto labelTitle = LabelBMFont::create("target", "fonts/boundsTestFont.fnt");
	labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	labelTitle->setPosition(Vec2(visibleOrigin.x + kLabelTitleX, visibleOrigin.y + kLabelTitleY));
	addChild(labelTitle);

	// label of score
	auto labelTarget = LabelBMFont::create("", "fonts/boundsTestFont.fnt");
	labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	labelTarget->setPosition(Vec2(visibleOrigin.x + kLabelScoreX, visibleOrigin.y + kLabelScoreY));
	addChild(labelTarget, 0, kLabelTargetTag);

	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	int score = roundInfo.m_targetScroe;
	int moves = roundInfo.m_totalMoves;
	labelTarget->setString("get at least " + StringUtils::toString(score) + " in " + StringUtils::toString(moves) + " moves");

	return true;
}