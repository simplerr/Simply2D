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
	virtual void draw(Game* game);
	void drawBkgd(void);

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