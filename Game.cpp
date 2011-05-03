//=============================================================================
// Game.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Demonstrates how to draw sprites with the ID3DXSprite interface, and how
// to move the sprite around and fire missiles with Direct Input using
// the mouse and keyboard.
//=============================================================================
#include "d3dApp.h"
#include "DirectInput.h"
#include "GfxStats.h"
#include <list>
#include <string>
#include "Game.h"
#include "Graphics.h"
#include <tchar.h>
#include "Window.h"
#include "Mouse.h"
#include "TextBox.h"
#include "InputBox.h"
#include "Enemy.h"
#include "CameraManager.h"
#include "MainMenuState.h"
#include "CustomEditorState.h"
#include "TestState.h"
#include "LevelCompletedState.h"
#include "StatsState.h"
#include "C:\Users\Axel\Documents\Visual Studio 2010\Memory_and_Exception_Trace\Stackwalker.h"


//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

// couldn't lie in d3dApp.cpp, no reason why not
extern CameraManager* gCameraManager;
extern Mouse* gMouse;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	InitAllocCheck(ACOutput_XML); // ACOutput_XML

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Game app(hInstance, "Simply2D", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

	/* this is where the game gets runned */
	return gd3dApp->run();
}

Game::Game(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	mGfxStats = new GfxStats();

	gMouse = new Mouse(mhMainWnd);

	gGraphics = new Graphics("bulle");

	// game and gui cameras!
	gCameraManager = new CameraManager();

	mGameState = NULL;

	changeState(MainMenuState::Instance());
	
	onResetDevice();

	//char *a = new char[200];
}

Game::~Game()
{
	mGameState->cleanup();

	delete mGfxStats;
	delete gMouse;
	delete gGraphics;
	delete gCameraManager;

	// print the memory leak file
	DeInitAllocCheck();
}

void Game::changeState(GameState* state)
{
	if(mGameState != NULL)
		mGameState->cleanup();

	mGameState = state;
	mGameState->init(this);

	// restores their positions
	gMouse->restore();
	gCameraManager->gameCamera()->restore();
}

bool Game::checkDeviceCaps()
{
	// Nothing to check.
	return true;
}

void Game::onLostDevice()
{
	//User->onLostDevice();
	mGfxStats->onLostDevice();
}

void Game::onResetDevice()
{
	// Call the onResetDevice of other objects.
	mGfxStats->onResetDevice();
	//User->onResetDevice();

	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);
	HR(gd3dDevice->SetTransform(D3DTS_WORLD, &W));

	// This code sets texture filters, which helps to smooth out distortions
	// when you scale a texture.  
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));

	//HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT));

	// This line of code disables Direct3D lighting
	//HR(gd3dDevice->SetRenderState(D3DRS_LIGHTING, false));
	
	// The following code specifies an alpha test and reference value.
	//HR(gd3dDevice->SetRenderState(D3DRS_ALPHAREF, 10));
	//HR(gd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER));

	gd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// The following code is used to setup alpha blending.
	//HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
	//HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1));
	//HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	//HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	// Indicates that we are using 2D texture coordinates.
	HR(gd3dDevice->SetTextureStageState(
		0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));
}

void Game::updateScene(float dt)
{		
	/* updates the mouse */
	gMouse->updateMouseDX();

	/* update the current state */
	mGameState->update(dt);

	/* this should allways be displayed */
	mGfxStats->setTriCount(8 *2);
	mGfxStats->setVertexCount(16 *4);
	mGfxStats->update(dt);
}

void Game::drawScene()
{
	HR(gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

	// clears the backbuffer and depth buffer
	gCameraManager->setCamera(GAME_CAMERA);

	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));	

	gCameraManager->setCamera(GUI_CAMERA);

	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	/* activate game camera */
	gCameraManager->setCamera(GAME_CAMERA);

	/* should allways be displayed */
	mGfxStats->display();

	/* draw the state content in the game area */
	mGameState->drawMain();

	/* activate gui camera */
	gCameraManager->setCamera(GUI_CAMERA);

	/* draw the state content in the gui area */
	mGameState->drawGui();

	gMouse->drawMousePos();
	
	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	D3DApp::msgProc(msg, wParam, lParam);

	mGameState->handleEvents(msg, wParam, lParam);

	return DefWindowProc(mhMainWnd, msg, wParam, lParam);
}