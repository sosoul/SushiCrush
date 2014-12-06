#ifndef __VIEW_LAYER_MAIN_LAYER_H__
#define __VIEW_LAYER_MAIN_LAYER_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"


USING_NS_CC;
class PlayLayer;
class TargetLayer;
class MovesLayer;
class ScoreLayer;
class TargetTipsLayer;

class MainLayer : public Layer
{
public:
	MainLayer();
	~MainLayer();
	CREATE_FUNC(MainLayer);
	virtual void onEnter() override;
	virtual bool init() override;

private:
	PlayLayer* m_playLayer;
	TargetLayer* m_targetLayer;
	MovesLayer* m_movesLayer;
	ScoreLayer* m_scoreLayer;
	TargetTipsLayer* m_targetTipsLayer;

	void onPlayLayerActionEnded();
	// notifications
	void onRoundReady(Ref* obj);
	void onRoundStart(Ref* obj);
	void onRoundEnd(Ref* obj);


	void onBackButtonTouched(Ref *pSender, ui::TouchEventType type);
	void onRefreshButtonTouched(Ref *pSender, ui::TouchEventType type);
	void onRoundStartActionEnd();
};

#endif // __VIEW_LAYER_MAIN_LAYER_H__
