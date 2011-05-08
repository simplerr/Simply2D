#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"
#include "C:\Program Files (x86)\irrKlang-1.3.0\include\irrKlang.h"

class Game;

// abstract baseclass
class GameState
{
public:
	virtual void init(Game* game) = 0;
	virtual void cleanup(void) = 0;
	virtual void shutdown(void)	{
		if(mMusic != NULL)	{
			mMusic->stop();
			mMusic->drop();
		}
	}

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void update(double dt) = 0;
	virtual void drawMain(void) = 0;
	virtual void drawGui(void) = 0;

	void setGame(Game* game) {mGame = game;};

	void setMusic(irrklang::ISound *music) { mMusic = music;}
	void setMusicVolume(float volume) { mMusic->setVolume(volume);}
	irrklang::ISound* getMusic(void) {return mMusic;}
	void stopMusic()	{
		mMusic->stop();
		mMusic->drop();
		mMusic = NULL;
	}

	void changeState(GameState* state) {
		mGame->changeState(state);
	}
protected:
	GameState(){};
private:
	bool mStateChanged;
	Game *mGame;
	irrklang::ISound *mMusic;
};

#endif