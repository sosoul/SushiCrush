#include "controller/GameController.h"

#include "TypeDefine.h"

namespace {

const int kPerSushiScore = 10;

GamController::GamController() : m_curRoundInfo(NULL) {
	init();
}
GamController::~GamController() {
	for (RoundInfoVecIt it = m_roundInfoList.begin(); it != m_roundInfoList.end(); ++it) {
		delete (*it);
		(*it) = NULL;
	}
	m_roundInfoList.clear();
}

void GamController::init() {
	int baseScore = 1000;
	int deltaScore = 100;
	int baseMoves = 10;
	int deltaMoves = 5;
	for (int i = 1; i <= TOTAL_ROUND; ++i) {
		RoundInfo* item = new RoundInfo();
		item->m_round = i;
		item->m_targetScroe = baseScore + i*i*deltaScore;
		item->m_totalMoves = baseMoves + i*i*deltaMoves;
		m_roundInfoList.push_back(item);
	}
}

void GamController::onRemoveSushiCompleted(int count) {
	// TODO

	m_curRoundInfo->m_leftMoves--;
	m_curRoundInfo->m_gotScore += count*kPerSushiScore;
	movesChanged(m_curRoundInfo->m_leftMoves);
	scoreChanged(m_curRoundInfo->m_gotScore);
	if (0 == m_curRoundInfo->m_leftMoves) {
		if (m_curRoundInfo->m_gotScore < m_curRoundInfo->m_targetScroe) {
			CCASSERT(false, "You lose!");
			return;
		}
			
		// achieve the target score, go into next round
		int round = m_curRoundInfo->m_round;
		++round;
		CCASSERT(round > 0, "Error round!");
		CCASSERT(round != TOTAL_ROUND, "Game Over.");
		writeToDB(m_curRoundInfo);
		m_curRoundInfo = m_roundInfoList[round];
		roundChanged(round);
	}
}

void GamController::roundChanged(int curRound) {
	NotificationCenter::getInstance()->postNotification(MSG_ROUND_CHANGED, (Ref*)(intptr_t)m_curRoundInfo->m_round);
}

void GamController::movesChanged(int leftMoves) {
	NotificationCenter::getInstance()->postNotification(MSG_MOVES_CHANGED, (Ref*)(intptr_t)m_curRoundInfo->m_leftMoves);
}

void GamController::scoreChanged(int gotScore) {
	NotificationCenter::getInstance()->postNotification(MSG_SCORE_CHANGED, (Ref*)(intptr_t)m_curRoundInfo->m_gotScore);
}

void GamController::writeToDB(RoundInfo* m_curRoundInfo) {
}

}
