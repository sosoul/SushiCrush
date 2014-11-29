#ifndef __VIEW_SCENE_GUIDE_MAP_SCENE_H__
#define __VIEW_SCENE_GUIDE_MAP_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GuideMapScene : public Scene
{
public:
	GuideMapScene();
	~GuideMapScene();
	CREATE_FUNC(GuideMapScene);

	virtual void onEnter() override;

private:

};

#endif // __VIEW_SCENE_GUIDE_MAP_SCENE_H__
