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

	virtual void onEnter() override;

	void onRoundEnd(Ref* obj);
	void onRoundReady(Ref* obj);
	void onRoundStart(Ref* obj);

private:
	PostPlayLayer* m_postPlayLayer;
	PrePlayLayer* m_prePlayLayer;
};

#endif // __VIEW_SCENE_PLAY_SCENE_H__
