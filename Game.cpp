//=============================================================================
// Game.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Demonstrates how to draw sprites with the ID3DXSprite interface, and how
// to move the sprite around and fire missiles with Direct Input using
// the mouse and keyboard.
//=============================================================================
#include "d3dApp.h"
#include "DirectInput.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>
#include <string>
//#include <fstream>
#include "Game.h"
#include "Graphics.h"
#include <tchar.h>
#include "Window.h"
#include "TextBox.h"
#include "InputBox.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Game app(hInstance, "Simply2D", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

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

	mMouse = new Mouse(mhMainWnd);
	mStartMenu = new Menu(mhMainWnd, "StartMenu", MOUSE, 4, 4);
	gGraphics = new Graphics("bulle");

	buildMainMenu();
	
	mGfxStats = new GfxStats();
	User = new Player("misc\\textures\\player.bmp", 200, 30, 36, 36);
	mLevel = new GameWorld(User);
	char buffer[256];
	strcpy(buffer, ACTIVE_LEVEL.c_str());
	mLevel->loadLevel(buffer);
	mEditor = new Editor();
	mEditor->addMouse(mMouse);
	mEditor->buildGUI();


	loadBkgd("misc\\textures\\cool_background.bmp");
		
	mainMenuActive = true;
	gameActive = false;
	editorActive = false;

	onResetDevice();
}

Game::~Game()
{
	mLevel->saveLevel("level_1.txt");

	delete mStartMenu;
	delete mGfxStats;
	delete mLevel;
	delete User;
	delete mMouse;
	delete mEditor;
}

bool Game::checkDeviceCaps()
{
	// Nothing to check.
	return true;
}

// L�gg till onLostDevice() f�r mLevel, Enemies och Player! 
void Game::onLostDevice()
{
	User->onLostDevice();
	mGfxStats->onLostDevice();
}

// l�gg till samma sak h�r >.<
void Game::onResetDevice()
{
	// Call the onResetDevice of other objects.
	mGfxStats->onResetDevice();
	User->onResetDevice();

	// Sets up the camera 1000 units back looking at the origin.
	D3DXMATRIX V;
	D3DXVECTOR3 pos(0.0f, 0.0f, -1000.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &V));

	// The following code defines the volume of space the camera sees.
	D3DXMATRIX P;
	RECT R;
	GetClientRect(mhMainWnd, &R);
	float width  = (float)R.right;
	float height = (float)R.bottom;
	D3DXMatrixPerspectiveFovLH(&P, D3DX_PI*0.25f, width/height, 1.0f, 5000.0f);
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &P));

	// This code sets texture filters, which helps to smooth out distortions
	// when you scale a texture.  
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));

	// This line of code disables Direct3D lighting.
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
	// updaterar musens position
	mMouse->updateMouseWIN();
	static float dtcount = 0;
	dtcount += dt;
	if(mainMenuActive && gDInput->keyPressed(DIK_ESCAPE))	{	// ska anpassas s� det funkar med editorActive ocks�!!
		mainMenuActive = false;
		gameActive = true;
		dtcount = 0;
	}
	else if(gameActive && gDInput->keyPressed(DIK_ESCAPE))	{
		mainMenuActive = true;
		gameActive = false;
		dtcount = 0;
	}
	else if(editorActive && gDInput->keyPressed(DIK_ESCAPE))	{
		mainMenuActive = true;
		editorActive = false;
		dtcount = 0;
	}
	string menuResult;
	if(mainMenuActive)
	{
		mStartMenu->updateMenu(mMouse->getPos());
		menuResult = mainMenuHandler();		// kolla knapptryckningar..!
		if(menuResult == "Play")	{
			gameActive = true;
			mainMenuActive = false;
			editorActive = false;
			sprintf(buffer, ACTIVE_LEVEL.c_str()); 
			mLevel->loadLevel(buffer);
		}
		else if(menuResult == "Editor")	{
			editorActive = true;
			mainMenuActive = false;
			gameActive = false;	
			sprintf(buffer, ACTIVE_LEVEL.c_str()); 
			mEditor->loadLevel(buffer);
		}
		// else if menuResult == "Options"
		// showOptions() ...

	}
	else if(gameActive)
	{										
		mGfxStats->setTriCount(8 *2);
		mGfxStats->setVertexCount(16 *4);
		mGfxStats->update(dt);
		
		User->update(dt, mLevel);
		mLevel->updateLevel(dt);
	}
	else if(editorActive)
	{
		mEditor->updateAll(dt);
	}
}

void Game::drawScene()
{
	// ----- Draw background	- DONE
	// ----- Draw player		- DONE
	// ----- Draw enemies
	// ----- Draw mLevel			- DONE

	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));
	HR(gd3dDevice->BeginScene());


	if(mainMenuActive)
	{
		mStartMenu->drawMenu();	
	}
	else if(gameActive)
	{
		drawBkgd();
		User->draw();
		mLevel->drawLevel();
		mGfxStats->display();	
	}
	// if (Builder) ->
	else if(editorActive)
	{	
		drawBkgd();
		mEditor->renderAll();		// renders ui + mLevel
		mGfxStats->display();			
	}
	mMouse->drawMousePos();
	
	HR(gd3dDevice->EndScene());
	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void Game::loadBkgd(char* filename)
{	
	mBkgdTex = gGraphics->loadTexture(filename); // ska ta filename egentligen^)
		
	if(!mBkgdTex)
		MessageBox(0, "Eror loading background texture", 0, 0);
}

// b�r vara sk�rmens storlek
void Game::drawBkgd()
{	
	RECT r1;
	r1.top = 0;
	r1.left = 0;
	r1.right = 1000;
	r1.bottom = 800;

	gGraphics->BlitTexture(mBkgdTex, r1, 0xFFFFFFFF, 0.0f);
}

void Game::buildMainMenu(void)
{
	mStartMenu->setMenuBackground("misc\\textures\\menubackground.bmp", 400, 300, 128, 256);
	mStartMenu->addMenuItem("Play", "misc\\textures\\play.bmp", "misc\\textures\\play_onselect.bmp", "misc\\textures\\play_onpress.bmp");	
	mStartMenu->addMenuItem("Editor", "misc\\textures\\editor.bmp", "misc\\textures\\editor_onselect.bmp", "misc\\textures\\options_onpress.bmp");
	mStartMenu->addMenuItem("Credits", "misc\\textures\\credits.bmp", "misc\\textures\\credits_onselect.bmp", "misc\\textures\\credits_onpress.bmp");
	mStartMenu->addMenuItem("Quit", "misc\\textures\\quit.bmp", "misc\\textures\\quit_onselect.bmp", "misc\\textures\\quit_onpress.bmp");
	mStartMenu->buildMenu();
}

std::string Game::mainMenuHandler(void)
{
	// update
	//mStartMenu->updateMouse();
	// get input
	if(mMouse->buttonDown(LEFTBUTTON))
	{
		if(mStartMenu->buttonPressed(mMouse->getPos(), "Play"))	{	
			return "Play";		
		}
		else if(mStartMenu->buttonPressed(mMouse->getPos(), "Editor"))	{
			return "Editor";
		}
		else if(mStartMenu->buttonPressed(mMouse->getPos(),  "Quit"))
			return "Quit";
	}

	return "NONE";
}

LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	D3DApp::msgProc(msg, wParam, lParam);

	switch( msg )
	{
	case WM_KEYDOWN:
		if( wParam == '1' || wParam == '2' || wParam == '3' || wParam == '4' || wParam == '5' || wParam == '6' || wParam == '7' || wParam == '8' || wParam == '9' ||
			wParam == '0' || wParam == '\b' || wParam == '\r' || wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_DELETE)
		{
			mEditor->keyPressed(wParam);
		}
	// case MOUSE?

		// skicka wParam till WindowHandler
		return 0;
	}
	return DefWindowProc(mhMainWnd, msg, wParam, lParam);
}