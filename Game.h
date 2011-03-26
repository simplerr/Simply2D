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
#include "Window.h"
#include "Mouse.h"
#include "Editor.h"

class Camera;

enum GameState
{
	MAIN_MENU_STATE = 100,
	PLAYING_STATE,
	TESTING_STATE,
	EDITOR_STATE
};

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

	void loadBkgd(char* filename);
	void drawBkgd();
	
	void buildMainMenu(void);
	std::string mainMenuHandler(void);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND input;

	GfxStats* mGfxStats;

	IDirect3DTexture9* mBkgdTex;
	D3DXVECTOR3 mBkgdCenter;

	Level *mLevel;
	Player *User;
	Editor *mEditor;

	Menu *mStartMenu;
	Mouse *mMouse;

	GameState mGameState;
	//bool mainMenuActive, gameActive, editorActive, testActive;
	//bool firstTime;		// for the mouse position
private:
	
};

#endif