#ifndef __CONTROLLER_GAME_CONTROLLER_H__
#define __CONTROLLER_GAME_CONTROLLER_H__

#include "Types.h"

#define TOTAL_ROUND	(10)

enum READY_ACTION_TYPE
{
	ACTION_RESUME = 0,
	ACTION_NEXT_ROUND,
};

class GameController : public Ref
{
public:
	static GameController *getInstance();
	static void destroyInstance();

	GameController();
	~GameController();

	void init();
	void uninit();
	const RoundInfo& get_cur_round_info() { return m_curRoundInfo; }
	RoundInfo* getRoundInfo(int round);
	void setCurRound(int round);

	void onSwapSushiCompleted();
	void onRemoveSushiCompleted(int count);
	void onExplosionStopped();
	void onRoundReady(READY_ACTION_TYPE actionType);
	void onRoundStart();

	void movesChanged(int leftMoves);
	void scoreChanged(int gotScore);

private:
	void writeToDB(const RoundInfo& m_curRoundInfo);

	typedef std::map<int, RoundInfo> RoundInfMap;
	typedef RoundInfMap::iterator RoundInfMapIt;
	RoundInfMap m_roundInfoMap;
	RoundInfo m_curRoundInfo;

};


#endif // __CONTROLLER_GAME_CONTROLLER_H__
