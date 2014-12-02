#ifndef __VIEW_LAYER_PRE_PLAY_LAYER_H__
#define __VIEW_LAYER_PRE_PLAY_LAYER_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"

class RoundInfo;

USING_NS_CC;

class PrePlayLayer : public Layer
{
public:
	PrePlayLayer();
	~PrePlayLayer();
	CREATE_FUNC(PrePlayLayer);

	virtual bool init() override;

	void start(Ref* object, ui::TouchEventType type);
	
private:
};

#endif // __VIEW_LAYER_PRE_PLAY_LAYER_H__
