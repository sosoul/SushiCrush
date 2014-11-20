#ifndef __VIEW_LAYER_MAIN_LAYER_H__
#define __VIEW_LAYER_MAIN_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class MainLayer : public Layer
{
public:
	MainLayer();
	~MainLayer();
	CREATE_FUNC(MainLayer);

	virtual bool init() override;

	// notifications
	void onRoundChanged(Ref* obj);

private:

};

#endif // __VIEW_LAYER_MAIN_LAYER_H__
