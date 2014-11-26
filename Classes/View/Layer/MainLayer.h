#ifndef __VIEW_LAYER_MAIN_LAYER_H__
#define __VIEW_LAYER_MAIN_LAYER_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"

USING_NS_CC;

class MainLayer : public Layer
{
public:
	MainLayer();
	~MainLayer();
	CREATE_FUNC(MainLayer);

	virtual bool init() override;

	// notifications
	void onRoundReady(Ref* obj);

private:
	void onBackToStartButtonTouched(Ref *pSender, ui::TouchEventType type);

};

#endif // __VIEW_LAYER_MAIN_LAYER_H__
