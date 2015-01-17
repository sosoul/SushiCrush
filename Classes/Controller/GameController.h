#ifndef __CONTROLLER_GAME_CONTROLLER_H__
#define __CONTROLLER_GAME_CONTROLLER_H__

#include "Common/Types.h"
#include "Common/ConfigService.h"

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
	MapTarget m_mapTarget;
	MapTarget m_mapGotTarget;

	CurRoundInfo() : m_round(0),
		m_totalMoves(0),
		m_leftMoves(0) {
		m_mapTarget.insert(MapTarget::value_type(TARGET_TYPE_SCORE, 0));
		m_mapTarget.insert(MapTarget::value_type(TARGET_TYPE_JELLY, 0));
		m_mapTarget.insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_JELLY, 0));
		m_mapTarget.insert(MapTarget::value_type(TARGET_TYPE_CREAM, 0));
		m_mapTarget.insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_CREAM, 0));

		m_mapGotTarget.insert(MapTarget::value_type(TARGET_TYPE_SCORE, 0));
		m_mapGotTarget.insert(MapTarget::value_type(TARGET_TYPE_JELLY, 0));
		m_mapGotTarget.insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_JELLY, 0));
		m_mapGotTarget.insert(MapTarget::value_type(TARGET_TYPE_CREAM, 0));
		m_mapGotTarget.insert(MapTarget::value_type(TARGET_TYPE_DOUBLE_CREAM, 0));
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

	void onSwapSushiCompleted();
	void onRemoveSushiCompleted(const MapTarget& map);
	void onExplosionStopped();
	void onRoundReady(READY_ACTION_TYPE actionType);
	void onRoundStart();

	void movesChanged(int leftMoves);
	void scoreChanged(int gotScore);
	void targetChanged();

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
