#include "View/Layer/PrePlayLayer.h"

#include "Controller/GameController.h"
#include "Messages.h"

namespace{
	const int kBackPrePlayX = 100;
	const int kBackYPrePlay = 100;
}

PrePlayLayer::PrePlayLayer()
{
}

PrePlayLayer::~PrePlayLayer()
{
}

bool PrePlayLayer::init()
{
	if (!Layer::init())
		return false;

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PrePlayLayer::onRoundChanged),
		MSG_ROUND_CHANGED, nullptr);

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("preplay/back.png");
	background->setScaleX(0.5);
	background->setScaleY(0.5);
	background->setPosition(Vec2(kBackPrePlayX, kBackYPrePlay));
	addChild(background);
	
	auto resumeBtn = Sprite::create("preplay/resume.png");
	resumeBtn->setScaleX(0.2);
	resumeBtn->setScaleY(0.2);
	resumeBtn->setPosition(Vec2(kBackPrePlayX, kBackYPrePlay));
	addChild(resumeBtn);

	auto nextBtn = Sprite::create("preplay/next.png");
	nextBtn->setScaleX(0.2);
	nextBtn->setScaleY(0.2);
	nextBtn->setPosition(Vec2(kBackPrePlayX, kBackYPrePlay));
	addChild(nextBtn);


	// ����һ���¼������� OneByOne Ϊ���㴥��
	auto resumelistener = EventListenerTouchOneByOne::create();
	// �����Ƿ���û�¼����� onTouchBegan �������� true ʱ��û
	resumelistener->setSwallowTouches(true);

	// ʹ�� lambda ʵ�� onTouchBegan �¼��ص�����
	resumelistener->onTouchBegan = [](Touch* touch, Event* event){
		// ��ȡ�¼����󶨵� target 
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		// ��ȡ��ǰ�����������԰�ť��λ������
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		// �����Χ�жϼ��
		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(resumelistener, resumeBtn);



	// ����һ���¼������� OneByOne Ϊ���㴥��
	auto nextlistener = EventListenerTouchOneByOne::create();
	// �����Ƿ���û�¼����� onTouchBegan �������� true ʱ��û
	nextlistener->setSwallowTouches(true);

	// ʹ�� lambda ʵ�� onTouchBegan �¼��ص�����
	nextlistener->onTouchBegan = [](Touch* touch, Event* event){
		// ��ȡ�¼����󶨵� target 
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		// ��ȡ��ǰ�����������԰�ť��λ������
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		// �����Χ�жϼ��
		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(nextlistener, nextBtn);


	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), sprite2);


	//// label "Target:"
	//auto labelTitle = LabelBMFont::create("Target:", "fonts/boundsTestFont.fnt");
	//labelTitle->setPosition(Vec2(kLabelTitleX, kLabelTitleY));
	//addChild(labelTitle, 0, kLabelTitleTag);

	//// label of target
	//auto labelTarget = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.plist");
	//labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//labelTarget->setPosition(Vec2(kLabelTargetX, kLabelTargetY));
	//addChild(labelTarget, 0, kLabelTargetTag);

	//// init target
	//const RoundInfo& roundInfo = GameController::getInstance()->get_cur_round_info();
	//onRoundChanged((Ref*)(intptr_t)roundInfo.m_round);



	return true;
}

void PrePlayLayer::onRoundChanged(Ref* obj)
{


	/*int round = (int)obj;
	RoundInfo* roundInfo = GameController::getInstance()->getRoundInfo(round);
	if (!roundInfo)
	return;
	int target = roundInfo->m_targetScroe;
	auto labelTarget = (LabelAtlas*)getChildByTag(kLabelTargetTag);
	labelTarget->setString(StringUtils::toString(target));*/
}