#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

class Game;

// abstract baseclass
class GameState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents(Game* game) = 0;
	virtual void update(Game* game, double dt) = 0;
	virtual void draw(Game* game) = 0;

	void ChangeState(Game* game, GameState* state) {
		game->changeState(state);
	}
protected:
	GameState(){};
private:
	bool mStateChanged;
};

#endif