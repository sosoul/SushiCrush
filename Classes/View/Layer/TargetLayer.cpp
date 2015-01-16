#include "View/Layer/TargetLayer.h"

#include "Common/Messages.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"

namespace {
const int kLabelTitleTag = 0;
const int kLabelTargetTag = 1;
const int kBackgroundX = 120;
const int kBackgroundY = -80;
const int kLabelTitleX = 1;
const int kLabelTitleY = -100;
const int kLabelTargetX = 120;
const int kLabelTargetY = -80;
}


TargetLayer::TargetLayer()
{
}

TargetLayer::~TargetLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool TargetLayer::init()
{
	if (!Layer::init())
		return false;

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(TargetLayer::onRoundEnd),
		MSG_ROUND_END, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(TargetLayer::onRoundReady),
		MSG_ROUND_READY, nullptr);

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(TargetLayer::onTargetChanged),
		MSG_TARGET_CHANGED, nullptr);

	// background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::createWithSpriteFrameName(s_target);
	background->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	background->setPosition(Vec2(visibleOrigin.x + kBackgroundX, visibleOrigin.y + visibleSize.height + kBackgroundY));
	addChild(background);

	// label "Target:"
	auto labelTitle = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
	labelTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	labelTitle->setPosition(Vec2(visibleOrigin.x + kLabelTitleX, visibleOrigin.y + visibleSize.height + kLabelTitleY));
	addChild(labelTitle, 0, kLabelTitleTag);

	// label of target
	// init target
	const int kX1 = visibleOrigin.x + 150;
	const int kX2 = kX1 + 150;
	const int kDeltaX = -40;
	const int kStartY = visibleOrigin.y + visibleSize.height - 100;
	const int kDeltaY = -40;
	int x = kX1;
	int y = kStartY;
	const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	const MapTarget& target = roundInfo.m_mapTarget;
	MapTarget::const_iterator it = target.find(TARGET_TYPE_SCORE);
	if (target.end() != it) {
		auto score_label = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		score_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		score_label->setPosition(Vec2(x, y));
		if (x == kX2)
			y += kDeltaY;
		addChild(score_label);
		score_label->setString(StringUtils::toString(it->second));
	}
	it = target.find(TARGET_TYPE_JELLY);
	if (target.end() != it) {
		auto jelly_label = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		jelly_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		x = (x == kX1 ? kX2 : kX1);
		jelly_label->setPosition(Vec2(x, y));
		addChild(jelly_label);
		jelly_label->setString("/ " + StringUtils::toString(it->second));

		got_jelly_label_ = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		got_jelly_label_->retain();
		got_jelly_label_->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		got_jelly_label_->setPosition(Vec2(x + kDeltaX, y));
		addChild(got_jelly_label_);
		got_jelly_label_->setString("0");

		if (x == kX2)
			y += kDeltaY;
	}
	it = target.find(TARGET_TYPE_DOUBLE_JELLY);
	if (target.end() != it) {
		auto double_jelly_label = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		double_jelly_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		x = (x == kX1 ? kX2 : kX1);
		double_jelly_label->setPosition(Vec2(x, y));
		addChild(double_jelly_label);
		double_jelly_label->setString("/ " + StringUtils::toString(it->second));

		got_double_jelly_label_ = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		got_double_jelly_label_->retain();
		got_double_jelly_label_->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		got_double_jelly_label_->setPosition(Vec2(x + kDeltaX, y));
		addChild(got_double_jelly_label_);
		got_double_jelly_label_->setString("0");

		if (x == kX2)
			y += kDeltaY;
	}
	it = target.find(TARGET_TYPE_CREAM);
	if (target.end() != it) {
		auto cream_label = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		cream_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		x = (x == kX1 ? kX2 : kX1);
		cream_label->setPosition(Vec2(x, y));
		addChild(cream_label);
		cream_label->setString("/ " + StringUtils::toString(it->second));

		got_cream_label_ = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		got_cream_label_->retain();
		got_cream_label_->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		got_cream_label_->setPosition(Vec2(x + kDeltaX, y));
		addChild(got_cream_label_);
		got_cream_label_->setString("0");

		if (x == kX2)
			y += kDeltaY;
	}
	it = target.find(TARGET_TYPE_DOUBLE_CREAM);
	if (target.end() != it) {
		auto double_cream_label = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		double_cream_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		x = (x == kX1 ? kX2 : kX1);
		double_cream_label->setPosition(Vec2(x, y));
		addChild(double_cream_label);
		double_cream_label->setString("/ " + StringUtils::toString(it->second));

		got_double_cream_label_ = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
		got_double_cream_label_->retain();
		got_double_cream_label_->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		got_double_cream_label_->setPosition(Vec2(x + kDeltaX, y));
		addChild(got_double_cream_label_);
		got_double_cream_label_->setString("0");

		if (x == kX2)
			y += kDeltaY;
	}
	
	return true;
}

void TargetLayer::onRoundEnd(Ref* obj)
{
	
}

void TargetLayer::onRoundReady(Ref* obj)
{
}

void TargetLayer::onTargetChanged(Ref* obj) {
	const CurRoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	const MapTarget& mapTarget = roundInfo.m_mapGotTarget;
	MapTarget::const_iterator it = mapTarget.find(TARGET_TYPE_JELLY);
	if (mapTarget.end() != it && got_jelly_label_) {
		got_jelly_label_->setString(StringUtils::toString(it->second));
	}
	it = mapTarget.find(TARGET_TYPE_DOUBLE_JELLY);
	if (mapTarget.end() != it && got_double_jelly_label_) {
		got_double_jelly_label_->setString(StringUtils::toString(it->second));
	}
	it = mapTarget.find(TARGET_TYPE_CREAM);
	if (mapTarget.end() != it && got_cream_label_) {
		got_cream_label_->setString(StringUtils::toString(it->second));
	}
	it = mapTarget.find(TARGET_TYPE_DOUBLE_CREAM);
	if (mapTarget.end() != it && got_double_cream_label_) {
		got_double_cream_label_->setString(StringUtils::toString(it->second));
	}
}