#ifndef LEVELCOMPLETEDSTATE_H
#define LEVELCOMPLETEDSTATE_H

#include "Window.h"
#include "GameState.h"

class WindowHandler;

class LevelCompletedState	:	public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void messageHandler(WindowID id, string data);
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void update(double dt);
	void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);

	static LevelCompletedState* Instance() {
		return &mLevelCompletedState;
	}
protected:
	LevelCompletedState() {};
private:
	static LevelCompletedState mLevelCompletedState;

	WindowHandler *mWindowHandler;
	IDirect3DTexture9* mBkgdTexture;
};

#endif