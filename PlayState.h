#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"

class PlayState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	virtual void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void update(double dt);
	void drawMain(void);
	virtual void drawGui(void);
	virtual void drawBkgd(void);
	void setLevel(string level);
	void setLevelType(LevelType type) {mLevel->setType(type);}
	string getLevel(void) { return mLevel->getLevelName();}

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