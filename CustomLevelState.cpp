#include "CustomLevelState.h"

#include "CameraManager.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "Menu.h"

class Menu;

extern CameraManager* gCameraManager;
extern Mouse* gMouse;

CustomLevelState CustomLevelState::mCustomLevelState;

void CustomLevelState::init()
{
	// create the menu
	mCustomLevelMenu = new Menu("CustemLevelMenu", MOUSE, false, 4, 4);

	mCustomLevelMenu->setMenuBackground("misc\\textures\\menubackground.bmp", 700, 450, 128, 256);
	mCustomLevelMenu->addMenuItem("Play", "misc\\textures\\level_menu_normal.bmp", "misc\\textures\\level_menu_hoover.bmp", "misc\\textures\\play_onpress.bmp");
	mCustomLevelMenu->buildMenu();
}

void CustomLevelState::cleanup()
{
	delete mCustomLevelMenu;
}

void CustomLevelState::pause()
{
	// don't know how to to yet
}

void CustomLevelState::resume()
{
	// don't know how to to yet
}

void CustomLevelState::handleEvents(Game* game)
{
	if(gDInput->keyPressed(DIK_ESCAPE))
		game->changeState(MainMenuState::Instance());
}

void CustomLevelState::update(Game* game, double dt)
{
	mCustomLevelMenu->update(gMouse->getPos());

	// check if the playe pressed a menu element
	string result = menuHandler();
}

void CustomLevelState::drawMain(Game* game)
{
	mCustomLevelMenu->draw();
}

void CustomLevelState::drawGui(Game* game)
{
	// the green side
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
}

string CustomLevelState::menuHandler(void)
{
	if(gMouse->buttonDown(LEFTBUTTON))
	{
		if(mCustomLevelMenu->buttonPressed(gMouse->getPos(), "CustemLevelMenu"))	{	
			return "CustemLevelMenu";		
		}	
	}

	return "none";
}