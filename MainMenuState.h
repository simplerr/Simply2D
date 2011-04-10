#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include "Menu.h"

class MainMenuState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void update(double dt);
	void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);

	bool menuHandler(std::string name);

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