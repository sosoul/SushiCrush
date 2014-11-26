#ifndef __VIEW_LAYER_POST_PLAY_LAYER_H__
#define __VIEW_LAYER_POST_PLAY_LAYER_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/UIButton.h"

USING_NS_CC;

class PostPlayLayer : public Layer
{
public:
	PostPlayLayer();
	~PostPlayLayer();
	CREATE_FUNC(PostPlayLayer);

	virtual bool init() override;

	void resume(Ref* object, ui::TouchEventType type);
	void nextRound(Ref* object, ui::TouchEventType type);

	void onTargetSuccess(Ref* obj);
	void onTargetFail(Ref* obj);

private:
};

#endif // __VIEW_LAYER_POST_PLAY_LAYER_H__
