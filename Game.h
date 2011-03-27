#ifndef GAME_H
#define GAME_H

#include "d3dApp.h"
#include "DirectInput.h"
#include <crtdbg.h>
#include <string>
#include "GfxStats.h"
#include <list>
#include "Level.h"
#include "Graphics.h"
#include "Menu.h"
//#include "GameState.h"
#include "Window.h"
#include "Editor.h"

class Camera;
class GameState;

class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~Game();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();

	void updateScene(float dt);
	void drawScene();

	void changeState(GameState* state);

	void loadBkgd(char* filename);
	void drawBkgd();
	
	//void buildMainMenu(void);
	//std::string mainMenuHandler(void);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND input;

	GfxStats* mGfxStats;

	GameState *mGameState;
};

#endif