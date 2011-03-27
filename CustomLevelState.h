#ifndef CUSTOMLEVELSTATE_H
#define CUSTOMLEVELSTATE_H

#include "GameState.h"
#include "Menu.h"

class CustomLevelState : public GameState
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

	static CustomLevelState* Instance() {
		return &mCustomLevelState;
	}

	string menuHandler(void);
protected:
	CustomLevelState() {}
private:
	static CustomLevelState mCustomLevelState;

	Menu *mCustomLevelMenu;
};

#endif