#include "View/Layer/TargetTipsLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"
#include "Resource.h"

namespace {
const int kLabelTargetTag = 0;
const int kBackgroundX = 470;
const int kBackgroundY = 370;
const int kLabelTitleX = 360;
const int kLabelTitleY = 400;
const int kLabelScoreX = 470;
const int kLabelScoreY = 370;
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
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(s_targetTipsBackground);
	background->setPosition(Vec2(kBackgroundX, kBackgroundY));
	addChild(background);
	// label "Score:"
	auto labelTitle = LabelBMFont::create("target", "fonts/boundsTestFont.fnt");
	labelTitle->setPosition(Vec2(kLabelTitleX, kLabelTitleY));
	addChild(labelTitle);

	// label of score
	auto labelTarget = LabelBMFont::create("target", "fonts/boundsTestFont.fnt");
	labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelTarget->setPosition(Vec2(kLabelScoreX, kLabelScoreY));
	addChild(labelTarget, 0, kLabelTargetTag);

	const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	int score = roundInfo.m_targetScroe;
	int moves = roundInfo.m_totalMoves;
	labelTarget->setString("get at least " + StringUtils::toString(score) + " in " + StringUtils::toString(moves) + " moves");

	return true;
}