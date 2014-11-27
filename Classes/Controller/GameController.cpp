#include "Controller/GameController.h"

#include "Messages.h"
#include "View/Layer/MainLayer.h"
#include "View/Layer/MovesLayer.h"
#include "View/Layer/ScoreLayer.h"
#include "View/Layer/TargetLayer.h"


const int kPerSushiScore = 10;

static GameController *s_sharedGameController = nullptr;

GameController::GameController() {
}
GameController::~GameController() {
	m_roundInfoMap.clear();
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
	int baseScore = 300;
	int deltaScore = 50;
	int baseMoves = 10;
	int deltaMoves = 1;
	for (int i = 1; i <= TOTAL_ROUND; ++i) {
		RoundInfo item;
		item.m_round = i;
		item.m_targetScroe = baseScore + i*i*deltaScore;
		item.m_totalMoves = baseMoves + i*i*deltaMoves;
		item.m_leftMoves = item.m_totalMoves;
		m_roundInfoMap[i] = item;
	}
	m_curRoundInfo = m_roundInfoMap[1];
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
		m_curRoundInfo = m_roundInfoMap[round];
	}
}

void GameController::onRoundReady(READY_ACTION_TYPE actionType) {
	if (ACTION_NEXT_ROUND == actionType) {
		int round = m_curRoundInfo.m_round;
		round++;
		CCASSERT(round >= 0, "Error round!");
		CCASSERT(round != TOTAL_ROUND, "Game Over.");
		writeToDB(m_curRoundInfo);
		m_curRoundInfo = m_roundInfoMap[round];
	}
	
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_READY, (Ref*)(&m_curRoundInfo));
}

void GameController::onRoundStart() {
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_START, (Ref*)(&m_curRoundInfo));
}

void GameController::onRemoveSushiCompleted(int count) {
	// TODO
	if (m_curRoundInfo.m_leftMoves == m_curRoundInfo.m_totalMoves)
	{
		return;
	}
	m_curRoundInfo.m_gotScore += count*kPerSushiScore;
	scoreChanged(m_curRoundInfo.m_gotScore);
	
}

void GameController::movesChanged(int leftMoves) {
	NotificationCenter::getInstance()->postNotification(MSG_MOVES_CHANGED, (Ref*)leftMoves);
}

void GameController::scoreChanged(int gotScore) {
	NotificationCenter::getInstance()->postNotification(MSG_SCORE_CHANGED, (Ref*)(intptr_t)gotScore);
}

void GameController::writeToDB(const RoundInfo& m_curRoundInfo) {
}

RoundInfo* GameController::getRoundInfo(int round) {
	RoundInfMapIt it = m_roundInfoMap.find(round);
	if (m_roundInfoMap.end() == it)
		return NULL;
	return &(it->second);
}

