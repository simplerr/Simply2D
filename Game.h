#ifndef GAME_H
#define GAME_H

#include "d3dApp.h"
#include "DirectInput.h"
#include <crtdbg.h>
#include <string>
#include "GfxStats.h"
#include <list>
#include "GameWorld.h"
#include "Graphics.h"
#include "Menu.h"
#include "Window.h"
#include "Mouse.h"
#include "Editor.h"

class Camera;

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

	void renderGame(void);
	void renderGUI(void);

	void loadBkgd(char* filename);
	void drawBkgd();
	
	void buildMainMenu(void);
	std::string mainMenuHandler(void);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	GfxStats* mGfxStats;

	IDirect3DTexture9* mBkgdTex;
	D3DXVECTOR3 mBkgdCenter;

	GameWorld *mLevel;
	Player *User;
	Editor *mEditor;

	Menu *mStartMenu;
	Mouse *mMouse;

	bool mainMenuActive, gameActive, editorActive, testActive;
private:
	
};

#endif