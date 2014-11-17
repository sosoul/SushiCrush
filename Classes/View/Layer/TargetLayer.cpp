#include "View/Layer/TargetLayer.h"

#include "TypeDefine.h"
#include "base/CCRef.h"

TargetLayer::TargetLayer()
{
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(TargetLayer::roundChanged), MSG_ROUND_CHANGED, nullptr);
}

TargetLayer::~TargetLayer()
{
}

bool TargetLayer::init()
{
	// TODO
	return Layer::init();
}

void TargetLayer::roundChanged(Ref* obj)
{
	// TODO
	// SetText
	// Sc
}