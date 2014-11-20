#ifndef __VIEW_SCENE_PLAY_SCENE_H__
#define __VIEW_SCENE_PLAY_SCENE_H__

#include "cocos2d.h"

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

private:

};

#endif // __VIEW_SCENE_PLAY_SCENE_H__
