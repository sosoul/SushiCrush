#include "Common/Utils.h"

void initTargetMap(MapTarget* map) {
	map->insert(MapTarget::value_type(TARGET_TYPE_SCORE, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_JELLY, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_JELLY, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_CREAM, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_CREAM, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_SUSHI_1, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_SUSHI_2, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_SUSHI_3, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_SUSHI_4, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_SUSHI_5, 0));
	map->insert(MapTarget::value_type(TARGET_TYPE_SUSHI_6, 0));
}

