#ifndef  _UTILS_H_
#define  _UTILS_H_

#include "cocos2d.h"
#include "Common/ConfigService.h"

USING_NS_CC;

MapTarget createTargetMap() {
	MapTarget map;
	map.insert(MapTarget::value_type(TARGET_TYPE_SCORE, 0));
	map.insert(MapTarget::value_type(TARGET_TYPE_JELLY, 0));
	map.insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_JELLY, 0));
	map.insert(MapTarget::value_type(TARGET_TYPE_CREAM, 0));
	map.insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_CREAM, 0));
	return map;
}

#endif // _UTILS_H_

