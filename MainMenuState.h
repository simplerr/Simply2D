#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include "Menu.h"

class MainMenuState : public GameState
{
public:
	void init();
	void cleanup();

	void pause();
	void resume();

	void handleEvents(Game* game);
	void update(Game* game, double dt);
	void drawMain(Game* game);
	void drawGui(Game* game);
	void drawBkgd(void);

	static MainMenuState* Instance() {
		return &mMainMenuState;
	}

	string menuHandler(void);
protected:
	MainMenuState() {};
private:
	static MainMenuState mMainMenuState;

	Menu *mStartMenu;	
};

#endif