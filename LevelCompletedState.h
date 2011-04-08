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

	bool messageHandler(WindowID id, string data);	// returns false if the state needs to change and the window loop needs to break
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void update(double dt);
	void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);
	void setCompletedType(LevelType type);
	void setLevels(string current, string next);

	static LevelCompletedState* Instance() {
		return &mLevelCompletedState;
	}
protected:
	LevelCompletedState() {};
private:
	static LevelCompletedState mLevelCompletedState;

	WindowHandler *mWindowHandler;
	IDirect3DTexture9* mBkgdTexture;
	LevelType mCompletedType;
	string mCurrentLevel;
	string mNextLevel;

private:
	/* windows */
	Button *mCustomNextButton;
	Button *mCampaignNextButton;
	Button *mMenuButton;
	Button *mAgainButton;
};

#endif