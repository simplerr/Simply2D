#ifndef CUSTOMLEVELSTATE_H
#define CUSTOMLEVELSTATE_H

#include "GameState.h"
#include "Menu.h"
#include "C:\Program Files (x86)\irrKlang-1.3.0\include\irrKlang.h"

class CustomLevelState : public GameState
{
public:
	virtual void init(Game* game);
	virtual void cleanup();

	void pause();
	void resume();

	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void update(double dt);
	void updateMenu(void);
	virtual void drawMain(void);
	void drawGui(void);
	void drawBkgd(void);
	void addItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource);
	void removeItem(std::string itemName);
	std::vector<string> getLevels(void);

	virtual bool menuHandler(std::string name);

	static CustomLevelState* Instance() {
		return &mCustomLevelState;
	}

protected:
	CustomLevelState() {}
	Menu *mCustomLevelMenu;
private:
	static CustomLevelState mCustomLevelState;
	IDirect3DTexture9 *mBackgroundTexture;
};

#endif