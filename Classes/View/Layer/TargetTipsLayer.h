#ifndef __VIEW_LAYER_TARGET_TIPS_LAYER_H__
#define __VIEW_LAYER_TARGET_TIPS_LAYER_H__

#include "cocos2d.h"
#include "Common/Types.h"

USING_NS_CC;

class TargetTipsLayer : public Layer
{
public:
	TargetTipsLayer(TipsLayerType type);
	~TargetTipsLayer();
	static TargetTipsLayer* create(TipsLayerType type);

	virtual bool init() override;

private:
	TipsLayerType _type;
};

#endif // __VIEW_LAYER_TARGET_TIPS_LAYER_H__
