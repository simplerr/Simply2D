#include "MainMenuState.h"
#include "CameraManager.h"
#include "PlayState.h"
#include "CustomLevelState.h"
#include "EditorState.h"
#include "CustomEditorState.h"
#include "CampaignLevelState.h"
#include "StatsState.h"
#include "Sound.h"
#include "OptionsState.h"

extern CameraManager* gCameraManager;

extern Sound* gSound;

MainMenuState MainMenuState::mMainMenuState;

void MainMenuState::init(Game* game)
{
	// important!
	setGame(game);

	// create the menu
	mStartMenu = new Menu("MainMenu", MOUSE, VER, 1, false, 4, 4);

	mStartMenu->setMenuBackground("none", 700, 450, 128, 350);
	mStartMenu->addMenuItem("Play", "misc\\textures\\play.bmp", "misc\\textures\\play_hoover.bmp");
	mStartMenu->addMenuItem("Custom", "misc\\textures\\custom.bmp", "misc\\textures\\custom_hoover.bmp");	
	mStartMenu->addMenuItem("Editor", "misc\\textures\\editor.bmp", "misc\\textures\\editor_hoover.bmp");
	mStartMenu->addMenuItem("Stats", "misc\\textures\\stats.bmp", "misc\\textures\\stats_hoover.bmp");
	mStartMenu->addMenuItem("Options", "misc\\textures\\options_normal.bmp", "misc\\textures\\options_hoover.bmp");
	mStartMenu->addMenuItem("Credits", "misc\\textures\\credits.bmp", "misc\\textures\\credits_hoover.bmp");
	mStartMenu->addMenuItem("Quit", "misc\\textures\\quit.bmp", "misc\\textures\\quit_hoover.bmp");
	
	mStartMenu->buildMenu2();
	mStartMenu->connect(&MainMenuState::menuHandler, this);

	mBackgroundTexture = gGraphics->loadTexture("misc\\textures\\epic_bkgd.bmp");

	gSound->playMusic("misc\\sound\\menu_loop.wav", true, true);
}

void MainMenuState::cleanup()
{
	delete mStartMenu;
	ReleaseCOM(mBackgroundTexture);
	//mMusic->stop();
	//mMusic->drop();
}

void MainMenuState::pause()
{
	// don't know how to to yet
}

void MainMenuState::resume()
{
	// don't know how to to yet
}

void MainMenuState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				changeState(MainMenuState::Instance());
		break;
	}
}

void MainMenuState::update(double dt)
{
	//mStartMenu->update(gMouse->getPos());
	mStartMenu->update(gDInput->getCursorX(), gDInput->getCursorY());
}

void MainMenuState::drawMain(void)
{
	gGraphics->BlitTexture(mBackgroundTexture, 0, 0, 1200, 900);
	mStartMenu->draw();	
}

void MainMenuState::drawGui(void)
{
	// the green side
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
}

/* the callback function for the menu */
bool MainMenuState::menuHandler(std::string name)
{
	if(name == "Play")
	{
		changeState(CampaignLevelState::Instance());
		return false;
	}
	else if(name == "Editor")
	{
		changeState(CustomEditorState::Instance());
		return false;
	}	
	else if(name == "Custom")
	{
		changeState(CustomLevelState::Instance());
		return false;
	}
	else if(name == "Quit")
	{
		// quit!
	}
	else if(name == "Stats")
	{
		changeState(StatsState::Instance());
		return false;
	}
	else if(name == "Options")
	{
		changeState(OptionsState::Instance());
		return false;
	}
	
	return true;
}