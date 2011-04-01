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
	mStartMenu->addMenuItem("Play", "misc\\textures\\play.bmp", "misc\\textures\\play_onselect.bmp", "misc\\textures\\play_onpress.bmp");
	mStartMenu->addMenuItem("Custom", "misc\\textures\\custom.bmp", "misc\\textures\\play_onselect.bmp", "misc\\textures\\play_onpress.bmp");	
	mStartMenu->addMenuItem("Editor", "misc\\textures\\editor.bmp", "misc\\textures\\editor_onselect.bmp", "misc\\textures\\options_onpress.bmp");
	mStartMenu->addMenuItem("Credits", "misc\\textures\\credits.bmp", "misc\\textures\\credits_onselect.bmp", "misc\\textures\\credits_onpress.bmp");
	mStartMenu->addMenuItem("Quit", "misc\\textures\\quit.bmp", "misc\\textures\\quit_onselect.bmp", "misc\\textures\\quit_onpress.bmp");
	mStartMenu->buildMenu();
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

	// check if the playe pressed a menu element
	string result = menuHandler();

	if(result == "Play")	{
		changeState(PlayState::Instance());	
	}
	else if(result == "Editor")	{
		changeState(CustomEditorState::Instance());
		//should ask for which level to edit -> SelectLevelState
	}
	else if(result == "Custom")	{
		changeState(CustomLevelState::Instance());
	}
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

string MainMenuState::menuHandler(void)
{
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

	return "none";
}