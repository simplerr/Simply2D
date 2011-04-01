#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

class Game;

// abstract baseclass
class GameState
{
public:
	virtual void init(Game* game) = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void update(double dt) = 0;
	virtual void drawMain(void) = 0;
	virtual void drawGui(void) = 0;

	void setGame(Game* game) {mGame = game;};

	void changeState(GameState* state) {
		mGame->changeState(state);
	}
protected:
	GameState(){};
private:
	bool mStateChanged;
	Game *mGame;
};

#endif