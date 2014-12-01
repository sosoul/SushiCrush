#ifndef __VIEW_SCENE_PLAY_SCENE_H__
#define __VIEW_SCENE_PLAY_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class PlayLayer;
class PrePlayLayer;
class PostPlayLayer;

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
	void onPlayLayerActionEnded();
	void onPrePlayLayerActionEnded();
	void onPostPlayLayerActionEnded();
	PlayLayer* m_playLayer;
	PostPlayLayer* m_postPlayLayer;
	PrePlayLayer* m_prePlayLayer;
};

#endif // __VIEW_SCENE_PLAY_SCENE_H__
