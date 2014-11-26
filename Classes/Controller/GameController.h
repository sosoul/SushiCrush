#ifndef __CONTROLLER_GAME_CONTROLLER_H__
#define __CONTROLLER_GAME_CONTROLLER_H__

#include "Types.h"
#include "View/Layer/PlayLayer.h"

#define TOTAL_ROUND	(10)

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

	void onSwapSushiCompleted();
	void onRemoveSushiCompleted(int count);
	void onActualMoveEnd();


	void onClickStart();
	void onClickResume();
	void onClickGoNextRound();

	void movesChanged(int leftMoves);
	void scoreChanged(int gotScore);


	void start();






private:
	void writeToDB(const RoundInfo& m_curRoundInfo);

	typedef std::map<int, RoundInfo> RoundInfMap;
	typedef RoundInfMap::iterator RoundInfMapIt;
	RoundInfMap m_roundInfoMap;
	RoundInfo m_curRoundInfo;

};


#endif // __CONTROLLER_GAME_CONTROLLER_H__
