#ifndef __VIEW_LAYER_START_LAYER_H__
#define __VIEW_LAYER_START_LAYER_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"

USING_NS_CC;

class StartLayer : public Layer
{
public:
	StartLayer();
	~StartLayer();
	CREATE_FUNC(StartLayer);

	virtual bool init() override;

private:
	void onStartButtonTouched(Ref *pSender, ui::TouchEventType type);
	void onMusicButtonTouched(Ref *pSender, ui::TouchEventType type);
	void onSoundButtonTouched(Ref *pSender, ui::TouchEventType type);
	void onRateButtonTouched(Ref *pSender, ui::TouchEventType type);

};

#endif // __VIEW_LAYER_START_LAYER_H__
