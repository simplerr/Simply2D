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
	void addItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource);
	std::vector<string> getLevels(void);

	bool menuHandler(std::string name);

	static CustomLevelState* Instance() {
		return &mCustomLevelState;
	}

	//string menuHandler(void);
protected:
	CustomLevelState() {}
	Menu *mCustomLevelMenu;
private:
	static CustomLevelState mCustomLevelState;
};

#endif