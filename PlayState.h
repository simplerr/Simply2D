#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"

class PlayState : public GameState
{
public:
	void init();
	void cleanup();

	void pause();
	void resume();

	virtual void handleEvents(Game* game);
	void update(Game* game, double dt);
	void drawMain(Game* game);
	virtual void drawGui(Game* game);
	virtual void drawBkgd(void);
	void setLevel(string level);

	static PlayState* Instance() {
		return &mPlayState;
	}
protected:
	PlayState() {};
private:
	static PlayState mPlayState;

	// game content
	Player *mPlayer;
	Level *mLevel;
	IDirect3DTexture9* mBkgdTexture;
};

#endif