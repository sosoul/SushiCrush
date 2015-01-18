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

const int kStartX = 50;
const int kEndX = kStartX + 300;
const int kDeltaX = 100;
const int kStartY = -110;
const int kDeltaY = -70;

}


TargetLayer::TargetLayer() : got_score_label_(nullptr),
							 got_jelly_label_(nullptr),
							 got_double_jelly_label_(nullptr),
							 got_cream_label_(nullptr),
							 got_double_cream_label_(nullptr),
							 got_sushi_1_label_(nullptr),
							 got_sushi_2_label_(nullptr),
							 got_sushi_3_label_(nullptr),
							 got_sushi_4_label_(nullptr),
							 got_sushi_5_label_(nullptr),
							 got_sushi_6_label_(nullptr)
{
}

TargetLayer::~TargetLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
	if (got_score_label_) {
		got_score_label_->release();
		got_score_label_ = nullptr;
	}
	if (got_jelly_label_) {
		got_jelly_label_->release();
		got_jelly_label_ = nullptr;
	}
	if (got_double_jelly_label_) {
		got_double_jelly_label_->release();
		got_double_jelly_label_ = nullptr;
	}
	if (got_cream_label_) {
		got_cream_label_->release();
		got_cream_label_ = nullptr;
	}
	if (got_double_cream_label_) {
		got_double_cream_label_->release();
		got_double_cream_label_ = nullptr;
	}
	if (got_sushi_1_label_) {
		got_sushi_1_label_->release();
		got_sushi_1_label_ = nullptr;
	}
	if (got_sushi_2_label_) {
		got_sushi_2_label_->release();
		got_sushi_2_label_ = nullptr;
	}
	if (got_sushi_3_label_) {
		got_sushi_3_label_->release();
		got_sushi_3_label_ = nullptr;
	}
	if (got_sushi_4_label_) {
		got_sushi_4_label_->release();
		got_sushi_4_label_ = nullptr;
	}
	if (got_sushi_5_label_) {
		got_sushi_5_label_->release();
		got_sushi_5_label_ = nullptr;
	}
	if (got_sushi_6_label_) {
		got_sushi_6_label_->release();
		got_sushi_6_label_ = nullptr;
	}
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

	createTargetPanels();

	return true;
}

void TargetLayer::createTargetPanels() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	int x = kStartX + visibleOrigin.x;
	int y = kStartY + visibleOrigin.y + visibleSize.height;
	int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SCORE);
	if (value) {
		createTargetPanel(&x, &y, value, s_scoreTarget, &got_score_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_JELLY);
	if (value) {
		createTargetPanel(&x, &y, value, s_jellyTarget, &got_jelly_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_DOUBLE_JELLY);
	if (value) {
		createTargetPanel(&x, &y, value, s_doubleJellyTarget, &got_double_jelly_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_CREAM);
	if (value) {
		createTargetPanel(&x, &y, value, s_creamTarget, &got_cream_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_DOUBLE_CREAM);
	if (value) {
		createTargetPanel(&x, &y, value, s_doubleCreamTarget, &got_double_cream_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_1);
	if (value) {
		createTargetPanel(&x, &y, value, s_sushi1Target, &got_sushi_1_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_2);
	if (value) {
		createTargetPanel(&x, &y, value, s_sushi2Target, &got_sushi_2_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_3);
	if (value) {
		createTargetPanel(&x, &y, value, s_sushi3Target, &got_sushi_3_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_4);
	if (value) {
		createTargetPanel(&x, &y, value, s_sushi4Target, &got_sushi_4_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_5);
	if (value) {
		createTargetPanel(&x, &y, value, s_sushi5Target, &got_sushi_5_label_);
	}
	value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_6);
	if (value) {
		createTargetPanel(&x, &y, value, s_sushi5Target, &got_sushi_6_label_);
	}
}

void TargetLayer::createTargetPanel(int* x, int* y, int value, const std::string image, LabelBMFont** targetLabel) {
	if (!x || !y || image.empty())
		return;

	// target icon
	auto icon = Sprite::create(image);
	icon->setPosition(Vec2(*x, *y));
	addChild(icon);

	// target numbers
	(*targetLabel) = LabelBMFont::create("", "fonts/boundsTestFont.fnt");
	(*targetLabel)->retain();
	(*targetLabel)->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	(*targetLabel)->setPosition(Vec2(*x, *y));
	(*targetLabel)->setString(StringUtils::toString(value));
	addChild((*targetLabel));
	
	if (*x == kEndX) {
		(*x) = kStartX;
		(*y) += kDeltaY;
	}
	else {
		(*x) += kDeltaX;
	}
}

void TargetLayer::onRoundEnd(Ref* obj)
{
	
}

void TargetLayer::onRoundReady(Ref* obj)
{
}

void TargetLayer::onTargetChanged(Ref* obj) {
	if (got_score_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SCORE) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SCORE);
		if (value < 0)
			value = 0;
		got_score_label_->setString(StringUtils::toString(value));
	}

	if (got_jelly_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_JELLY) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_JELLY);
		if (value < 0)
			value = 0;
		got_jelly_label_->setString(StringUtils::toString(value));
	}

	if (got_double_jelly_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_DOUBLE_JELLY) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_DOUBLE_JELLY);
		if (value < 0)
			value = 0;
		got_double_jelly_label_->setString(StringUtils::toString(value));
	}

	if (got_cream_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_CREAM) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_CREAM);
		if (value < 0)
			value = 0;
		got_cream_label_->setString(StringUtils::toString(value));
	}

	if (got_double_cream_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_CREAM) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_CREAM);
		if (value < 0)
			value = 0;
		got_double_cream_label_->setString(StringUtils::toString(value));
	}

	if (got_sushi_1_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_1) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SUSHI_1);
		if (value < 0)
			value = 0;
		got_sushi_1_label_->setString(StringUtils::toString(value));
	}

	if (got_sushi_2_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_2) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SUSHI_2);
		if (value < 0)
			value = 0;
		got_sushi_2_label_->setString(StringUtils::toString(value));
	}

	if (got_sushi_3_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_3) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SUSHI_3);
		if (value < 0)
			value = 0;
		got_sushi_3_label_->setString(StringUtils::toString(value));
	}

	if (got_sushi_4_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_4) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SUSHI_4);
		if (value < 0)
			value = 0;
		got_sushi_4_label_->setString(StringUtils::toString(value));
	}

	if (got_sushi_5_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_5) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SUSHI_5);
		if (value < 0)
			value = 0;
		got_sushi_5_label_->setString(StringUtils::toString(value));
	}

	if (got_sushi_6_label_) {
		int value = GameController::getInstance()->getTargetValue(TARGET_TYPE_SUSHI_6) -
			GameController::getInstance()->getGotTargetValue(TARGET_TYPE_SUSHI_6);
		if (value < 0)
			value = 0;
		got_sushi_6_label_->setString(StringUtils::toString(value));
	}
}