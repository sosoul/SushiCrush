#ifndef __VIEW_LAYER_SELECT_ROUND_LAYER_H__
#define __VIEW_LAYER_SELECT_ROUND_LAYER_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"

USING_NS_CC;

class SelectRoundLayer : public Layer
{
public:
	SelectRoundLayer();
	~SelectRoundLayer();
	CREATE_FUNC(SelectRoundLayer);

	virtual bool init() override;
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event) override;
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event) override;
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event) override;

	void onRoundButtonTouched(Ref *pSender, ui::TouchEventType type);

private:
	void createRoundButton(int round, bool isUnlock, const Point& pos);
	void onBackButtonTouched(Ref *pSender, ui::TouchEventType type);
};

class GuideMapOptionsLayer : public Layer
{
public:
	GuideMapOptionsLayer();
	~GuideMapOptionsLayer();
	CREATE_FUNC(GuideMapOptionsLayer);

	virtual bool init() override;

private:
	void onBackButtonTouched(Ref *pSender, ui::TouchEventType type);
};


#endif // __VIEW_LAYER_SELECT_ROUND_LAYER_H__
