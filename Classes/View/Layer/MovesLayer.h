#ifndef __VIEW_LAYER_MOVES_LAYER_H__
#define __VIEW_LAYER_MOVES_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class MovesLayer : public Layer
{
public:
	MovesLayer();
	~MovesLayer();
	CREATE_FUNC(MovesLayer);

	virtual bool init() override;

	// notifications
	void onRoundChanged(Ref* obj);
	void onMovesChanged(Ref* obj);

private:
	LabelAtlas* label_;
};

#endif // __VIEW_LAYER_MOVES_LAYER_H__
