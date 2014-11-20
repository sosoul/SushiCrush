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

#endif // _TYPES_H_

