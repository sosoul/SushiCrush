#ifndef __VIEW_LAYER_SCORE_LAYER_H__
#define __VIEW_LAYER_SCORE_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class ScoreLayer : public Layer
{
public:
	ScoreLayer();
	~ScoreLayer();
	CREATE_FUNC(ScoreLayer);

	virtual bool init() override;

private:

};

#endif // __VIEW_LAYER_SCORE_LAYER_H__
