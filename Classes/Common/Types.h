#ifndef  _TYPES_H_
#define  _TYPES_H_

#include "cocos2d.h"

USING_NS_CC;

struct RoundInfo : public Ref {
	int m_round;
	int m_totalMoves;
	int m_leftMoves;
	int m_gotScore;
	int m_targetScroe;

	RoundInfo() : m_round(0),
		m_totalMoves(0),
		m_leftMoves(0),
		m_gotScore(0),
		m_targetScroe(0) {
	}
};

typedef enum {
	DISPLAY_MODE_NORMAL = 0,
	DISPLAY_MODE_4_HORIZONTAL_LINE,
	DISPLAY_MODE_4_VERTICAL_LINE,
	DISPLAY_MODE_5_LINE,
	DISPLAY_MODE_5_CROSS,
	DISPLAY_MODE_7_T_SHAPED,
} DisplayMode;


enum GridType {
	GIRD_TYPE_NONE = 0,
	GIRD_TYPE_NORMAL,  // 1
	GRID_TYPE_JELLY,  // 2
	GRID_TYPE_DOUBLE_JELLY,  // 3
	GRID_TYPE_CREAM,  // 4
	GRID_TYPE_DOUBLE_CREAM,  // 5
};

#endif // _TYPES_H_

