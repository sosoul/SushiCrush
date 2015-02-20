#ifndef __VIEW_SCENE_SELECT_ROUND_SCENE_H__
#define __VIEW_SCENE_SELECT_ROUND_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class SelectRoundScene : public Scene
{
public:
	SelectRoundScene();
	~SelectRoundScene();
	CREATE_FUNC(SelectRoundScene);

	virtual void onEnter() override;

private:

};

#endif // __VIEW_SCENE_SELECT_ROUND_SCENE_H__
