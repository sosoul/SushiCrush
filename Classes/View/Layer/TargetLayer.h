#ifndef __VIEW_LAYER_TARGET_LAYER_H__
#define __VIEW_LAYER_TARGET_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class TargetLayer : public Layer
{
public:
	TargetLayer();
	~TargetLayer();
	CREATE_FUNC(TargetLayer);

	virtual bool init() override;

	void onRoundEnd(Ref* obj);
	void onRoundReady(Ref* obj);
	void onTargetChanged(Ref* obj);

private:
	void createTargetPanels();
	void createTargetPanel(int* x, int* y, int value, const std::string image, LabelBMFont** targetLabel);

	LabelBMFont* got_score_label_;
	LabelBMFont* got_jelly_label_;
	LabelBMFont* got_double_jelly_label_;
	LabelBMFont* got_cream_label_;
	LabelBMFont* got_double_cream_label_;

};

#endif // __VIEW_LAYER_TARGET_LAYER_H__
