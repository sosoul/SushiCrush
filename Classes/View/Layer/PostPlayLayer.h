#ifndef __VIEW_LAYER_POST_PLAY_LAYER_H__
#define __VIEW_LAYER_POST_PLAY_LAYER_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"

USING_NS_CC;

class PostPlayLayer : public Layer
{
public:
	PostPlayLayer(bool result);
	~PostPlayLayer();
	static PostPlayLayer* create(bool result);

	virtual bool init() override;

	void resume(Ref* object, ui::TouchEventType type);
	void nextRound(Ref* object, ui::TouchEventType type);
	void backToStartMap(Ref* object, ui::TouchEventType type);
private:
	bool _result;
};

#endif // __VIEW_LAYER_POST_PLAY_LAYER_H__
