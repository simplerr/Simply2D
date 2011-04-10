#include "MainMenuState.h"
#include "CameraManager.h"
#include "PlayState.h"
#include "CustomLevelState.h"
#include "EditorState.h"
#include "CustomEditorState.h"

extern CameraManager* gCameraManager;
extern Mouse* gMouse;

MainMenuState MainMenuState::mMainMenuState;

void MainMenuState::init(Game* game)
{
	// important!
	setGame(game);

	// create the menu
	mStartMenu = new Menu("StartMenu", MOUSE, false, 4, 4);

	mStartMenu->setMenuBackground("misc\\textures\\menubackground.bmp", 700, 450, 128, 256);
	mStartMenu->addMenuItem("Play", "misc\\textures\\play.bmp", "misc\\textures\\play_onselect.bmp");
	mStartMenu->addMenuItem("Custom", "misc\\textures\\custom.bmp", "misc\\textures\\play_onselect.bmp");	
	mStartMenu->addMenuItem("Editor", "misc\\textures\\editor.bmp", "misc\\textures\\editor_onselect.bmp");
	mStartMenu->addMenuItem("Credits", "misc\\textures\\credits.bmp", "misc\\textures\\credits_onselect.bmp");
	mStartMenu->addMenuItem("Quit", "misc\\textures\\quit.bmp", "misc\\textures\\quit_onselect.bmp");
	mStartMenu->buildMenu();
	mStartMenu->connect(&MainMenuState::menuHandler, this);
}

void MainMenuState::cleanup()
{
	delete mStartMenu;
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
	mStartMenu->update(gMouse->getPos());
}

void MainMenuState::drawMain(void)
{
	gGraphics->BlitRect(600, 450, 1200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
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
		changeState(PlayState::Instance());
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
	
	return true;
}