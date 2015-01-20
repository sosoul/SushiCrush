#include "Controller/GameController.h"


#include "Common/Messages.h"
#include "Common/Resource.h"

static GameController *s_sharedGameController = nullptr;

GameController::GameController() {
}
GameController::~GameController() {
}

// static
GameController* GameController::getInstance() {
	if (!s_sharedGameController)
	{
		s_sharedGameController = new GameController();
	}
	return s_sharedGameController;
}

void GameController::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_sharedGameController);
}

void GameController::init() {
	setCurRound(0);
	ReadUnlockInfo();
}

void GameController::uninit() {
}

void GameController::onSwapSushiCompleted() {
		m_curRoundInfo.m_leftMoves--;
		movesChanged(m_curRoundInfo.m_leftMoves);
}

void GameController::onRoundEnd() {
	if (!isPass(m_curRoundInfo.m_round)) {
		NotificationCenter::getInstance()->postNotification(MSG_ROUND_END, (Ref*)(false));
		return;
	}
	// achieve the target score store relate data
	int round = m_curRoundInfo.m_round;
	CCASSERT(round >= 0, "Error round!");
	CCASSERT(round != TOTAL_ROUND, "Game Over.");
	writeToDB(m_curRoundInfo);
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_END, (Ref*)(true));
	UpdateUnlockInfo(round+1, true);
}

void GameController::onRoundReady(READY_ACTION_TYPE actionType) {
	int round = m_curRoundInfo.m_round;
	if (ACTION_NEXT_ROUND == actionType) {
		round++;
		CCASSERT(round >= 0, "Error round!");
		CCASSERT(round != TOTAL_ROUND, "Game Over.");
		writeToDB(m_curRoundInfo);
	}
	setCurRound(round);
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_READY, (Ref*)(&m_curRoundInfo));
}

void GameController::onRoundStart() {
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_START, (Ref*)(&m_curRoundInfo));
}

void GameController::onRemoveSushiCompleted(const MapTarget& map) {
	// TODO
	if (m_curRoundInfo.m_leftMoves == m_curRoundInfo.m_totalMoves)
	{
		return;
	}
	MapTarget new_map = map;
	bool need_notify = false;
	
	if (0 != new_map[TARGET_TYPE_SCORE]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SCORE] += new_map[TARGET_TYPE_SCORE];
		scoreChanged(m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SCORE]);
		need_notify = true;
	}
	
	if (0 != new_map[TARGET_TYPE_JELLY]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_JELLY] += new_map[TARGET_TYPE_JELLY];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_DOUBLE_JELLY]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_DOUBLE_JELLY] += new_map[TARGET_TYPE_DOUBLE_JELLY];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_CREAM]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_CREAM] += new_map[TARGET_TYPE_CREAM];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_DOUBLE_CREAM]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_DOUBLE_CREAM] += new_map[TARGET_TYPE_DOUBLE_CREAM];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_SUSHI_1]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_1] += new_map[TARGET_TYPE_SUSHI_1];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_SUSHI_2]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_2] += new_map[TARGET_TYPE_SUSHI_2];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_SUSHI_3]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_3] += new_map[TARGET_TYPE_SUSHI_3];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_SUSHI_4]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_4] += new_map[TARGET_TYPE_SUSHI_4];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_SUSHI_5]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_5] += new_map[TARGET_TYPE_SUSHI_5];
		need_notify = true;
	}
	if (0 != new_map[TARGET_TYPE_SUSHI_6]) {
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_6] += new_map[TARGET_TYPE_SUSHI_6];
		need_notify = true;
	}

	if (need_notify)
		targetChanged();
}

void GameController::movesChanged(int leftMoves) {
	NotificationCenter::getInstance()->postNotification(MSG_MOVES_CHANGED, (Ref*)leftMoves);
}

void GameController::scoreChanged(int gotScore) {
	NotificationCenter::getInstance()->postNotification(MSG_SCORE_CHANGED, (Ref*)(intptr_t)gotScore);
}

void GameController::targetChanged() {
	NotificationCenter::getInstance()->postNotification(MSG_TARGET_CHANGED, (Ref*)nullptr);
}

void GameController::writeToDB(const CurRoundInfo& m_curRoundInfo) {
}

void GameController::setCurRound(int round) {
	CCASSERT(round >= 0 && round < TOTAL_ROUND, "round is out of range!");
	resetRoundInfo(round);
}

void GameController::ReadUnlockInfo() {
	for (int i = 0; i < TOTAL_ROUND; ++i)
		_roundUnlock.insert(MapRoundUnLock::value_type(i, false));
	_roundUnlock[0] = true;
}

void GameController::UpdateUnlockInfo(int round, bool isUnlock) {
	CCASSERT(round >= 0 && round < TOTAL_ROUND, "round is out of range!");
	_roundUnlock[round] = isUnlock;
}

bool GameController::isUnlock(int round) {
	CCASSERT(round >= 0 && round < TOTAL_ROUND, "round is out of range!");
	return _roundUnlock[round];
}

int GameController::getTargetValue(TargetType type) {
	return m_curRoundInfo.m_mapTarget[type];
}

int GameController::getGotTargetValue(TargetType type) {
	return m_curRoundInfo.m_mapGotTarget[type];
}

void GameController::resetRoundInfo(int round) {
	CCASSERT(round >= 0 && round < TOTAL_ROUND, "round is out of range!");
	const RoundInfo* roundInfo = ConfigService::getInstance()->getRoundInfo(round);
	m_curRoundInfo.m_round = round;
	m_curRoundInfo.m_mapTarget = roundInfo->_mapTarget;
	m_curRoundInfo.m_mapGotTarget.clear();
	m_curRoundInfo.m_totalMoves = roundInfo->_moves;
	m_curRoundInfo.m_leftMoves = roundInfo->_moves;
	m_curCrashMode = CRASH_MODE_NORMAL;
}

bool GameController::isPass(int round) {
	if (m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SCORE] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_SCORE] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_JELLY] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_JELLY] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_DOUBLE_JELLY] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_DOUBLE_JELLY] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_CREAM] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_CREAM] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_DOUBLE_CREAM] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_DOUBLE_CREAM] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_1] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_SUSHI_1] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_2] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_SUSHI_2] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_3] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_SUSHI_3] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_4] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_SUSHI_4] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_5] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_SUSHI_5] &&
		m_curRoundInfo.m_mapGotTarget[TARGET_TYPE_SUSHI_6] >= m_curRoundInfo.m_mapTarget[TARGET_TYPE_SUSHI_6]) {
		return true;
	}
	return false;
}

CRASH_MODE GameController::getCurCrashMode()
{
	return m_curCrashMode;
}

void GameController::changeCurCrashMode()
{
	if (m_curCrashMode == CRASH_MODE_NORMAL)
	{
		m_curCrashMode = CRASH_MODE_REMOVE_SPECIAL_SUSHI;
	}
	else if (m_curCrashMode == CRASH_MODE_REMOVE_SPECIAL_SUSHI)
	{
		m_curCrashMode = CRASH_MODE_GENERATE_SPECIAL_SUSHI;
	}
	else
	{
		m_curCrashMode == CRASH_MODE_NORMAL;
	}
}

void GameController::onTargetCompleted() {
	//����Ӧ��ĳЩ�Ȼ�������ٵ�������ķ���
	if (m_curCrashMode == CRASH_MODE_NORMAL)
	{
		//����Ŀ�����
		NotificationCenter::getInstance()->postNotification(MSG_TARGET_COMPLATE, (Ref*)(true));
	}
}

void GameController::onCrushBegin() {
	if (m_curCrashMode == CRASH_MODE_REMOVE_SPECIAL_SUSHI)
	{
		NotificationCenter::getInstance()->postNotification(MSG_CRASH_BEGIN, (Ref*)(true));
	}
}