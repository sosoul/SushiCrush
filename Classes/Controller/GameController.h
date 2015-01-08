#ifndef __CONTROLLER_GAME_CONTROLLER_H__
#define __CONTROLLER_GAME_CONTROLLER_H__

#include "Common/Types.h"

#define TOTAL_ROUND	(10)

enum READY_ACTION_TYPE
{
	ACTION_RESUME = 0,
	ACTION_NEXT_ROUND,
};

struct CurRoundInfo : public Ref {
	int m_round;
	int m_totalMoves;
	int m_leftMoves;
	int m_gotScore;
	int m_targetScroe;

	CurRoundInfo() : m_round(0),
		m_totalMoves(0),
		m_leftMoves(0),
		m_gotScore(0),
		m_targetScroe(0) {
	}
};

typedef std::map<int, bool> MapRoundUnLock;

class GameController : public Ref
{
public:
	static GameController *getInstance();
	static void destroyInstance();

	GameController();
	~GameController();

	void init();
	void uninit();
	const CurRoundInfo& get_cur_round_info() { return m_curRoundInfo; }
	void setCurRound(int round);

	void onSwapSushiCompleted();
	void onRemoveSushiCompleted(int score);
	void onExplosionStopped();
	void onRoundReady(READY_ACTION_TYPE actionType);
	void onRoundStart();

	void movesChanged(int leftMoves);
	void scoreChanged(int gotScore);

	void ReadUnlockInfo();
	void UpdateUnlockInfo(int round, bool isUnlock);
	bool isUnlock(int round);
private:
	void writeToDB(const CurRoundInfo& m_curRoundInfo);

	CurRoundInfo m_curRoundInfo;
	int m_curRound;
	MapRoundUnLock _roundUnlock;
};


#endif // __CONTROLLER_GAME_CONTROLLER_H__
