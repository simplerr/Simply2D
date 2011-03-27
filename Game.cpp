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
#include "Game.h"
#include "Graphics.h"
#include <tchar.h>
#include "Window.h"
#include "Mouse.h"
#include "TextBox.h"
#include "InputBox.h"
#include "Enemy.h"
#include "Camera.h"
#include "MainMenuState.h"

using namespace std;

extern Camera* gGameCamera;
extern Camera* gGuiCamera;
extern Mouse* gMouse;

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

	gMouse = new Mouse(mhMainWnd);

	gGraphics = new Graphics("bulle");

	// game and gui cameras!
	gGameCamera = new Camera(600, 450, 1200, 900);
	gGuiCamera = new Camera(1300, 450, 200, 900);

	mGameState = NULL;

	changeState(MainMenuState::Instance());
	
	onResetDevice();
}

Game::~Game()
{
	mGameState->cleanup();
}

void Game::changeState(GameState* state)
{
	if(mGameState != NULL)
		mGameState->cleanup();

	mGameState = state;
	mGameState->init();

	// restores their positions
	gMouse->restore();
	gGameCamera->restore();
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
	// updaterar musens position
	gMouse->updateMouseDX();

	// update the current state
	mGameState->handleEvents(this);
	mGameState->update(this, dt);

	// this should allways be displayed
	mGfxStats->setTriCount(8 *2);
	mGfxStats->setVertexCount(16 *4);
	mGfxStats->update(dt);


	/*switch(mGameState)
	{
	case MAIN_MENU_STATE:
		{
			string menuResult;
			mStartMenu->updateMenu(gMouse->getPos());
			menuResult = mainMenuHandler();
			if(menuResult == "Play")	{
				mGameState = PLAYING_STATE;
				sprintf(buffer, ACTIVE_LEVEL.c_str()); 
				mLevel->loadLevel(buffer);
				//mLevel->spawnPlayer(); //-> currently increases sides in shape
			}
			else if(menuResult == "Editor")	{
				mGameState = EDITOR_STATE;
				sprintf(buffer, ACTIVE_LEVEL.c_str()); 
				mEditor->loadLevel(buffer);
			}
			else if(menuResult == "Custom")	{
				mGameState = SELECTING_LEVEL;
			}

			if(gDInput->keyPressed(DIK_ESCAPE))
				mGameState = PLAYING_STATE;

			break;
		}
	case PLAYING_STATE:
		{
			mGfxStats->setTriCount(8 *2);
			mGfxStats->setVertexCount(16 *4);
			mGfxStats->update(dt);
		
			mLevel->updateLevel(dt);	// update objects + player
			gGameCamera->move();		// move the camera accordingly

			if(gDInput->keyPressed(DIK_ESCAPE))	{
				// restores if the camera have been moved
				gMouse->restore();
				gGameCamera->restore();
				mGameState = MAIN_MENU_STATE;
			}

			break;
		}
	case TESTING_STATE:
		{
			mGfxStats->setTriCount(8 *2);
			mGfxStats->setVertexCount(16 *4);
			mGfxStats->update(dt);
		
			mLevel->updateLevel(dt);	// update objects + player
			gGameCamera->move();		// move the camera accordingly

			if(gDInput->keyPressed(DIK_ESCAPE))	{
				// restores if the camera have been moved
				gMouse->restore();
				gGameCamera->restore();
				mEditor->setTest(false);
				mGameState = EDITOR_STATE;
			}

			break;
		}
	case EDITOR_STATE:
		{
			if(mEditor->updateAll(dt) < 0)	{
				sprintf(buffer, ACTIVE_LEVEL.c_str()); 
				mLevel->loadLevel(buffer);
				mLevel->spawnPlayer();
				mGameState = TESTING_STATE;
			}
			gGameCamera->move();

			if(gDInput->keyPressed(DIK_ESCAPE))	{
				// restores if the camera have been moved
				gMouse->restore();
				gGameCamera->restore();
				mGameState = MAIN_MENU_STATE;
			}
			break;
		}
	case SELECTING_LEVEL:
		{
			mSelectLevelMenu->updateMenu(gMouse->getPos());
			break;
		}
	}*/
}

void Game::drawScene()
{

	HR(gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

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

	// activate game camera
	if(!gGameCamera->getActive())	{
			gGameCamera->activate(true);
			gGuiCamera->activate(false);
		}

	// should allways be displayed
	mGfxStats->display();

	mGameState->draw(this);

	gMouse->drawMousePos();



	/*if(mGameState == MAIN_MENU_STATE)
	{
		if(!gGameCamera->getActive())	{
			gGameCamera->activate(true);
			gGuiCamera->activate(false);
		}

		gGraphics->BlitRect(600, 450, 1200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
		mStartMenu->drawMenu();	

		if(!gGuiCamera->getActive())	{
			gGuiCamera->activate(true);
			gGameCamera->activate(false);
		}

		HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));
		gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
		
	}
	else if(mGameState == PLAYING_STATE || mGameState == TESTING_STATE)
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
		gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
	}
	else if(mGameState == EDITOR_STATE)
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
	else if(mGameState == SELECTING_LEVEL)
	{
		if(!gGameCamera->getActive())
			gGameCamera->activate(true);
		mSelectLevelMenu->drawMenu();
	}

	if(mGameState == TESTING_STATE)
		gGraphics->drawText("Press ESC to return", 1020, 20);
	gMouse->drawMousePos();*/
	
	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

/*void Game::loadBkgd(char* filename)
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
	r1.right = 4800;
	r1.bottom = 900;

	D3DXMATRIX texScaling;
	D3DXMatrixScaling(&texScaling, 4.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));

	gGraphics->BlitTexture(mBkgdTex, r1, 0xFFFFFFFF, 0.0f);

	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));
}

void Game::buildMainMenu(void)
{
	mStartMenu->setMenuBackground("misc\\textures\\menubackground.bmp", 700, 450, 128, 256);
	mStartMenu->addMenuItem("Play", "misc\\textures\\play.bmp", "misc\\textures\\play_onselect.bmp", "misc\\textures\\play_onpress.bmp");
	mStartMenu->addMenuItem("Custom", "misc\\textures\\custom.bmp", "misc\\textures\\play_onselect.bmp", "misc\\textures\\play_onpress.bmp");	
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
	if(gMouse->buttonDown(LEFTBUTTON))
	{
		if(mStartMenu->buttonPressed(gMouse->getPos(), "Play"))	{	
			return "Play";		
		}
		else if(mStartMenu->buttonPressed(gMouse->getPos(), "Editor"))	{
			return "Editor";
		}
		else if(mStartMenu->buttonPressed(gMouse->getPos(),  "Quit"))
			return "Quit";
		else if(mStartMenu->buttonPressed(gMouse->getPos(), "Custom"))	{
			return "Custom";
		}
	}

	return "NONE";
}*/

LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	D3DApp::msgProc(msg, wParam, lParam);

	switch( msg )
	{
	case WM_KEYDOWN:
		//if( wParam == '1' || wParam == '2' || wParam == '3' || wParam == '4' || wParam == '5' || wParam == '6' || wParam == '7' || wParam == '8' || wParam == '9' ||
		//	wParam == '0' || wParam == '\b' || wParam == '\r' || wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_DELETE)
		//{
			//mEditor->keyPressed(wParam);
		//}
	// case MOUSE?

		// skicka wParam till WindowHandler
		return 0;
	}
	return DefWindowProc(mhMainWnd, msg, wParam, lParam);
}