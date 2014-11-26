#ifndef __VIEW_SCENE_START_SCENE_H__
#define __VIEW_SCENE_START_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Scene
{
public:
	StartScene();
	~StartScene();
	CREATE_FUNC(StartScene);

	virtual void onEnter() override;

	void run();

private:

};

#endif // __VIEW_SCENE_START_SCENE_H__
