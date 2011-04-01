#ifndef CUSTOMLEVELSTATE_H
#define CUSTOMLEVELSTATE_H

#include "GameState.h"
#include "Menu.h"

class CustomLevelState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void update(double dt);
	void updateMenu(void);
	void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);
	std::vector<string> getLevels(void);

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