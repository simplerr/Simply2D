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
#include "Enemy.h"
#include "Camera.h"

using namespace std;

extern Camera* gGameCamera;
extern Camera* gGuiCamera;

// couldn't lie in d3dApp.cpp, no reason why not

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

	mGfxStats = new GfxStats();

	mMouse = new Mouse(mhMainWnd);
	mStartMenu = new Menu(mhMainWnd, "StartMenu", MOUSE, 4, 4);
	gGraphics = new Graphics("bulle");

	// game and gui cameras!
	gGameCamera = new Camera(500, 400, 1000, 800);
	gGuiCamera = new Camera(1100, 400, 200, 800);

	buildMainMenu();
	
	User = new Player("misc\\textures\\player.bmp", USER_WIDTH, USER_HEIGHT);
	mLevel = new Level(User);
	mEditor = new Editor();
	mEditor->addMouse(mMouse);
	mEditor->buildGUI();

	loadBkgd("misc\\textures\\city_bkgd_yellow.bmp");
		
	mainMenuActive = true;
	gameActive = false;
	editorActive = false;
	testActive = false;
	firstTime = true;

	//mMouse->setMousePos(mMou
	//gGameCamera->activate();
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
	// ta reda på musens startPos vid första loopen
	if(firstTime)	{
		mMouse->restore();
		firstTime = false;
	}
	// updaterar musens position
	mMouse->updateMouseDX();
	static float dtcount = 0;
	dtcount += dt;
	if(mainMenuActive && gDInput->keyPressed(DIK_ESCAPE))	{	// ska anpassas s� det funkar med editorActive ocks�!!
		mainMenuActive = false;
		gameActive = true;
		dtcount = 0;
	}
	else if(gameActive && gDInput->keyPressed(DIK_ESCAPE))	{
		// restores if the camera have been moved
		mMouse->restore();
		gGameCamera->restore();
		mainMenuActive = true;
		gameActive = false;
		dtcount = 0;
	}
	else if(editorActive && gDInput->keyPressed(DIK_ESCAPE))	{
		mMouse->restore();
		gGameCamera->restore();
		mEditor->resetActive();
		mainMenuActive = true;
		editorActive = false;
		dtcount = 0;
	}
	else if(testActive && gDInput->keyPressed(DIK_ESCAPE))	{
		mMouse->restore();
		gGameCamera->restore();
		mEditor->setTest(false);
		testActive = false;
		editorActive = true;
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
			//mLevel->spawnPlayer(); //-> currently increases sides in shape
		}
		else if(menuResult == "Editor")	{
			editorActive = true;
			mainMenuActive = false;
			gameActive = false;	
			sprintf(buffer, ACTIVE_LEVEL.c_str()); 
			mEditor->loadLevel(buffer);
			// gameActive = true
		}
		// else if menuResult == "Options"
		// showOptions() ...

	}
	else if(gameActive || testActive)
	{			
		mGfxStats->setTriCount(8 *2);
		mGfxStats->setVertexCount(16 *4);
		mGfxStats->update(dt);
		
		mLevel->updateLevel(dt);	// update objects + player
		gGameCamera->move();		// move the camera accordingly
	}
	else if(editorActive)
	{
		// returnera TEST_LEVEL 
		if(mEditor->updateAll(dt) < 0)	{
			sprintf(buffer, ACTIVE_LEVEL.c_str()); 
			mLevel->loadLevel(buffer);
			mLevel->spawnPlayer();
			testActive = true;
			editorActive = false;
		}
		gGameCamera->move();
	}
	/*else if(testActive)
	{
		mGfxStats->setTriCount(8 *2);
		mGfxStats->setVertexCount(16 *4);
		mGfxStats->update(dt);

		mLevel->updateLevel(dt);
	}*/
}

void Game::drawScene()
{

	HR(gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

	//renderGame();
	//renderGUI();

	// Clear the backbuffer and depth buffer.
	// maybe unnecessary test, check it later..
	if(!gGameCamera->getActive())	{
			gGameCamera->activate(true);
			gGuiCamera->activate(false);
		}

	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));	

		if(!gGuiCamera->getActive())	{
			gGuiCamera->activate(true);
			gGameCamera->activate(false);
		}

	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	if(mainMenuActive)
	{
		if(!gGameCamera->getActive())	{
			gGameCamera->activate(true);
			gGuiCamera->activate(false);
		}

		gGraphics->BlitRect(600, 400, 1200, 800, D3DCOLOR_ARGB( 155, 155, 200, 000));
		mStartMenu->drawMenu();	

		if(!gGuiCamera->getActive())	{
			gGuiCamera->activate(true);
			gGameCamera->activate(false);
		}

		HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));
		gGraphics->BlitRect(1100, 400, 200, 800, D3DCOLOR_ARGB( 155, 155, 200, 000));
		
	}
	else if(gameActive || testActive)
	{
		if(!gGameCamera->getActive())	{
			gGameCamera->activate(true);
			gGuiCamera->activate(false);
		}
		drawBkgd();
		mLevel->drawLevel();
		mGfxStats->display();

		if(!gGuiCamera->getActive())	{
			gGuiCamera->activate(true);
			gGameCamera->activate(false);
		}
		gGraphics->BlitRect(1100, 400, 200, 800, D3DCOLOR_ARGB( 155, 155, 200, 000));
	}
	else if(editorActive)
	{	
		// render level
		if(!gGameCamera->getActive())	{
			gGameCamera->activate(true);
			gGuiCamera->activate(false);
		}

		drawBkgd();
		mGfxStats->display();
		mEditor->renderLevel();	

		// renders ui
		if(!gGuiCamera->getActive())	{
			gGuiCamera->activate(true);
			gGameCamera->activate(false);
		}
		mEditor->renderGui();				
	}

	if(testActive)
		gGraphics->drawText("Press ESC to return", 1020, 20);
	mMouse->drawMousePos();
	
	HR(gd3dDevice->EndScene());
	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

// render all the game content - not the gui
void Game::renderGame(void)
{
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	//	TEST
	//gGraphics->BlitRect(500, 400, 1000, 600, D3DCOLOR_ARGB(255, 255, 0, 0));

	if(mainMenuActive)
	{
		//gGraphics->BlitRect(500, 400, 200, 300, D3DCOLOR_ARGB( 255, 155, 200, 255));
		mStartMenu->drawMenu();	
	}
	else if(gameActive || testActive)
	{
		drawBkgd();
		mLevel->drawLevel();
		mGfxStats->display();
		gGraphics->BlitRect(1100, 400, 200, 800, D3DCOLOR_ARGB( 155, 155, 200, 000));
	}
	else if(editorActive)
	{	
		drawBkgd();
		mEditor->renderLevel();
		mGfxStats->display();			
	}

	if(testActive)
		gGraphics->drawText("Press ESC to return", 1020, 20);
	mMouse->drawMousePos();

	HR(gd3dDevice->EndScene());
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

// render all the GUI content
void Game::renderGUI(void)
{
	gd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,0,255), 1.0f, 0 );

	HR(gd3dDevice->BeginScene());
	
	if(editorActive)
		mEditor->renderAll();	// renders ui + mLevel
	
	if(gameActive || testActive)
	{
		gGraphics->BlitRect(1100, 400, 200, 800, D3DCOLOR_ARGB( 155, 155, 200, 000));
	}

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
	r1.right = 4000;
	r1.bottom = 800;

	D3DXMATRIX texScaling;
	D3DXMatrixScaling(&texScaling, 4.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));

	gGraphics->BlitTexture(mBkgdTex, r1, 0xFFFFFFFF, 0.0f);

	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));
}

void Game::buildMainMenu(void)
{
	mStartMenu->setMenuBackground("misc\\textures\\menubackground.bmp", 600, 400, 128, 256);
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