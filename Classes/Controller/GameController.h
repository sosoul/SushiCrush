#ifndef __CONTROLLER_GAME_CONTROLLER_H__
#define __CONTROLLER_GAME_CONTROLLER_H__

#include "Common/Types.h"
#include "Common/ConfigService.h"

#define TOTAL_ROUND	(51)

enum READY_ACTION_TYPE
{
	ACTION_RESUME = 0,
	ACTION_NEXT_ROUND,
};

struct CurRoundInfo : public Ref {
	int m_round;
	int m_totalMoves;
	int m_leftMoves;
	MapTarget m_mapTarget;
	MapTarget m_mapGotTarget;

	CurRoundInfo() : m_round(0),
		m_totalMoves(0),
		m_leftMoves(0) {
		initTargetMap(&m_mapTarget);
		initTargetMap(&m_mapGotTarget);
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
	int getTargetValue(TargetType type);
	int getGotTargetValue(TargetType type);
	void setCurRound(int round);

	// notifications
	void onSwapSushiCompleted();
	void onRemoveSushiCompleted(const MapTarget& map);
	void onRoundEnd();
	void onRoundReady(READY_ACTION_TYPE actionType);
	void onRoundStart();
	void onTargetCompleted();
	void onCrushBegin();

	void movesChanged(int leftMoves);
	void scoreChanged(int gotScore);
	void targetChanged();

	void ReadUnlockInfo();
	void UpdateUnlockInfo(int round, bool isUnlock);
	bool isUnlock(int round);
	bool isTargetCompleted(int round);
	CrushMode getCurCrushMode();
	void changeCurCrushMode();
private:
	void writeToDB(const CurRoundInfo& m_curRoundInfo);
	void resetRoundInfo(int round);

	CurRoundInfo m_curRoundInfo;
	int m_curRound;
	CrushMode m_curCrushMode;
	MapRoundUnLock _roundUnlock;
};


#endif // __CONTROLLER_GAME_CONTROLLER_H__
