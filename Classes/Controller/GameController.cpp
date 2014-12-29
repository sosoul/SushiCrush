#include "Controller/GameController.h"

#include "Common/ConfigService.h"
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
	s_sharedGameController->retain();
	return s_sharedGameController;
}

void GameController::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_sharedGameController);
}

void GameController::init() {
	setCurRound(0);
}

void GameController::uninit() {
}

void GameController::onSwapSushiCompleted() {
	m_curRoundInfo.m_leftMoves--;
	movesChanged(m_curRoundInfo.m_leftMoves);
	
}

void GameController::onExplosionStopped() {
	if (0 == m_curRoundInfo.m_leftMoves) {
		if (m_curRoundInfo.m_gotScore < m_curRoundInfo.m_targetScroe) {
			NotificationCenter::getInstance()->postNotification(MSG_ROUND_END, (Ref*)(false));
			return;
		}
		// achieve the target score store relate data
		int round = m_curRoundInfo.m_round;
		CCASSERT(round >= 0, "Error round!");
		CCASSERT(round != TOTAL_ROUND, "Game Over.");
		writeToDB(m_curRoundInfo);
		NotificationCenter::getInstance()->postNotification(MSG_ROUND_END, (Ref*)(true));
		setCurRound(round);
	}
}

void GameController::onRoundReady(READY_ACTION_TYPE actionType) {
	if (ACTION_NEXT_ROUND == actionType) {
		int round = m_curRoundInfo.m_round;
		round++;
		CCASSERT(round >= 0, "Error round!");
		CCASSERT(round != TOTAL_ROUND, "Game Over.");
		writeToDB(m_curRoundInfo);
		setCurRound(round);
	}
	
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_READY, (Ref*)(&m_curRoundInfo));
}

void GameController::onRoundStart() {
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_START, (Ref*)(&m_curRoundInfo));
}

void GameController::onRemoveSushiCompleted(int score) {
	// TODO
	if (m_curRoundInfo.m_leftMoves == m_curRoundInfo.m_totalMoves)
	{
		return;
	}
	m_curRoundInfo.m_gotScore += score;
	scoreChanged(m_curRoundInfo.m_gotScore);
	
}

void GameController::movesChanged(int leftMoves) {
	NotificationCenter::getInstance()->postNotification(MSG_MOVES_CHANGED, (Ref*)leftMoves);
}

void GameController::scoreChanged(int gotScore) {
	NotificationCenter::getInstance()->postNotification(MSG_SCORE_CHANGED, (Ref*)(intptr_t)gotScore);
}

void GameController::writeToDB(const CurRoundInfo& m_curRoundInfo) {
}

void GameController::setCurRound(int round) {
	CCASSERT(round >= 0 && round < TOTAL_ROUND, "round is out of range!");
	const RoundInfo* roundInfo = ConfigService::getInstance()->getRoundInfo(round);
	m_curRoundInfo.m_round = round;
	m_curRoundInfo.m_gotScore = 0;
	m_curRoundInfo.m_targetScroe = roundInfo->_targetScore;
	m_curRoundInfo.m_totalMoves = roundInfo->_moves;
	m_curRoundInfo.m_leftMoves = roundInfo->_moves;
}
