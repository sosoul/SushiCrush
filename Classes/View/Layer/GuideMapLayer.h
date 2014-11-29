#ifndef __VIEW_LAYER_GUIDE_MAP_LAYER_H__
#define __VIEW_LAYER_GUIDE_MAP_LAYER_H__

#include "cocos2d.h"
#include "deprecated/CCDictionary.h"
#include "ui/UIWidget.h"

USING_NS_CC;

class GuideMapLayer : public Layer
{
public:
	GuideMapLayer();
	~GuideMapLayer();
	CREATE_FUNC(GuideMapLayer);

	virtual bool init() override;
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event) override;
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event) override;
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event) override;

	void onRoundButtonTouched(Ref *pSender, ui::TouchEventType type);

private:
	void createRoundButton(int round);
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


#endif // __VIEW_LAYER_GUIDE_MAP_LAYER_H__
