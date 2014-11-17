#ifndef __CONTROLLER_GAME_CENTER_OBSERVER_H__
#define __CONTROLLER_GAME_CENTER_OBSERVER_H__

class GameCenterObserver
{
public:
	GameCenterObserver() {}
	~GameCenterObserver() {}

	virtual OnOneMoveFinished(int moves, int score) {}
	virtual OnRoundEnded(bool result) {}
};

#endif // __CONTROLLER_GAME_CENTER_OBSERVER_H__
