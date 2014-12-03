#ifndef __VIEW_SCENE_PLAY_SCENE_H__
#define __VIEW_SCENE_PLAY_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class MainLayer;
class PrePlayLayer;
class PostPlayLayer;
class TargetTipsLayer;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();
	CREATE_FUNC(PlayScene);

	virtual bool init() override;
	virtual void onEnter() override;

	void onRoundEnd(Ref* obj);
	void onRoundReady(Ref* obj);
	void onRoundStart(Ref* obj);

private:
	void onPlayLayerActionEnded();
	void onPrePlayLayerActionEnded();
	void onPostPlayLayerActionEnded();

	PostPlayLayer* m_postPlayLayer;
	PrePlayLayer* m_prePlayLayer;
	MainLayer* m_mainLayer;

};

#endif // __VIEW_SCENE_PLAY_SCENE_H__
