#ifndef __CONTROLLER_GAME_CONTROLLER_H__
#define __CONTROLLER_GAME_CONTROLLER_H__

#include "View/Layer/PlayLayer.h"

namespace {

#define TOTAL_ROUND	(10)

class GamController : public PlayLayer::Delegate
{
public:
	struct RoundInfo {
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

	GamController();
	~GamController();

	void init();

	// Overridden from Player::Delegate
	virtual void onRemoveSushiCompleted(int count) override;

	void roundChanged(int curRound);
	void movesChanged(int leftMoves);
	void scoreChanged(int gotScore);

private:
	void writeToDB(RoundInfo* m_curRoundInfo);

	typedef std::vector<RoundInfo*> RoundInfoVec;
	typedef RoundInfoVec::iterator RoundInfoVecIt;
	RoundInfoVec m_roundInfoList;
	RoundInfo* m_curRoundInfo;

};

}  // namespace

#endif // __CONTROLLER_GAME_CONTROLLER_H__
