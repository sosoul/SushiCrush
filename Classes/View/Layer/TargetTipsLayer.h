#ifndef __VIEW_LAYER_TARGET_TIPS_LAYER_H__
#define __VIEW_LAYER_TARGET_TIPS_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class TargetTipsLayer : public Layer
{
public:
	TargetTipsLayer();
	~TargetTipsLayer();
	CREATE_FUNC(TargetTipsLayer);

	virtual bool init() override;

private:

};

#endif // __VIEW_LAYER_TARGET_TIPS_LAYER_H__
