#ifndef __VIEW_SCENE_PLAY_SCENE_H__
#define __VIEW_SCENE_PLAY_SCENE_H__

#include "cocos2d.h"
#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "Common/Types.h"

USING_NS_CC;

class MainLayer;
class PrePlayLayer;
class PostPlayLayer;
class TargetTipsLayer;
class PlayLayer;
class TargetLayer;
class MovesLayer;
class ScoreLayer;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();
	CREATE_FUNC(PlayScene);

	virtual bool init() override;
	virtual void onEnter() override;

private:
	void onPrePlayLayerActionEnded();
	void onPostPlayLayerActionEnded();

	PostPlayLayer* m_postPlayLayer;
	PrePlayLayer* m_prePlayLayer;
	//MainLayer* m_mainLayer;

	// notifications
	void onRoundReady(Ref* obj);
	void onRoundStart(Ref* obj);
	void onRoundEnd(Ref* obj);

	// button touch events
	void onBackButtonTouched(Ref *pSender, ui::TouchEventType type);
	void onRefreshButtonTouched(Ref *pSender, ui::TouchEventType type);

	// animation callback
	void onPlayLayerActionEnded();
	void onTargetLayerActionEnded();
	void OnTargetTipsActionEnd();
	//void OnSuccessfulTipsActionEnd();
	//void OnFailTipsActionEnd();
	void OnResultTipsActionEnd(bool result);

	void showTipsLayer(TipsLayerType type, CallFunc* func);

	PlayLayer* m_playLayer;
	TargetLayer* m_targetLayer;
	MovesLayer* m_movesLayer;
	ScoreLayer* m_scoreLayer;
	TargetTipsLayer* m_tipsLayer;

};

#endif // __VIEW_SCENE_PLAY_SCENE_H__
