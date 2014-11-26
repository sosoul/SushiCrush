#ifndef __VIEW_SCENE_PLAY_SCENE_H__
#define __VIEW_SCENE_PLAY_SCENE_H__

#include "cocos2d.h"
#include "View/Layer/PostPlayLayer.h"
#include "View/Layer/PrePlayLayer.h"
USING_NS_CC;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();
	CREATE_FUNC(PlayScene);

	static Scene* createScene();

	virtual void onEnter() override;

	void onRoundCompleted(Ref* obj);
	void onSuccess(Ref* obj);

	void onResume(Ref* obj);
	void onNextRound(Ref* obj);
	void onStart(Ref* obj);

private:
	PostPlayLayer* postPlayLayer;
	PrePlayLayer* prePlayLayer;
};

#endif // __VIEW_SCENE_PLAY_SCENE_H__
