#include "CustomLevelState.h"
#include <Windows.h>
#include <direct.h>
#include <tchar.h>
#include <string>
#include <algorithm>
#include "CameraManager.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "CustomEditorState.h"
#include "Menu.h"
#include "TestState.h"


class Menu;

extern CameraManager* gCameraManager;
extern Mouse* gMouse;

CustomLevelState CustomLevelState::mCustomLevelState;
CustomEditorState CustomEditorState::mCustomEditorState;
TestState TestState::mTestState;

void CustomLevelState::init(Game* game)
{
	// important!
	setGame(game);

	// create the menu
	//mCustomLevelMenu = new Menu("CustemLevelMenu", MOUSE, true, 4, 4);
	mCustomLevelMenu = new Menu("CustemLevelMenu", MOUSE, VER, 2, true, 4, 4);
	mCustomLevelMenu->setMenuBackground("misc\\textures\\menu_bkgd.bmp", 700, 450, 256, 350);

	// get the files in the map folder
	std::vector<string> levelList = getLevels();

	// add menu items, each level
	for(int i = 0; i < levelList.size(); i++)
	{
		// remove .txt
		if(levelList[i].find(".txt") != string::npos || levelList[i].find(".TXT") != string::npos)
			levelList[i].erase(levelList[i].end()-4,levelList[i].end()); 

		mCustomLevelMenu->addMenuItem(levelList[i], "misc\\textures\\level_menu_normal.bmp", "misc\\textures\\level_menu_hoover.bmp");
	}

	// build the menu
	mCustomLevelMenu->buildMenu2();
	mCustomLevelMenu->connect(&CustomLevelState::menuHandler, this);
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

void CustomLevelState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				changeState(MainMenuState::Instance());
		break;
	}
}

void CustomLevelState::update(double dt)
{
	// call menuHandler() when a item is pressed
	// state changing takes place there
	updateMenu();
}

// just so CustomEditorState can reach this
void CustomLevelState::updateMenu(void)
{
	// consider to combine this with the menuHandler()
	mCustomLevelMenu->update(gMouse->getPos());
}

void CustomLevelState::drawMain(void)
{
	gGraphics->BlitRect(600, 450, 1200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
	mCustomLevelMenu->draw();
}

void CustomLevelState::drawGui(void)
{
	// the green side
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
}

std::vector<string> CustomLevelState::getLevels(void)
{
	int size;
	int counter = 0;
	bool working(true);
	string test;
	char buffer[256] = "";
	std::vector<string> levelList;

	WIN32_FIND_DATA findData;
	HANDLE myHandle = FindFirstFile("levels\\*",&findData); // C:\\hlserver\\*

	if (myHandle == INVALID_HANDLE_VALUE) 
	{
		 MessageBox(0, "Error loading the first file", 0, 0);
	} 
	else 
	{
		do
		{
			test = string(findData.cFileName);

			if (test != "."  && test !=  ".." && test != "campaign")
			{
				// remove spaces
				for(int j = 0; j < test.length()-1; j++)	{
					if(test[j] == ' ')
						test.erase(j);
				}
				levelList.push_back(test);
				counter++;
			}
		}	while (FindNextFile(myHandle, &findData));
	}

	return levelList;
}

void CustomLevelState::addItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource)
{
	mCustomLevelMenu->addMenuItem(itemName, standardTextureSource, onSelectTextureSource);
	mCustomLevelMenu->buildMenu2();	
}

bool CustomLevelState::menuHandler(std::string name)
{	
	//add levels\\ and .txt 
	string tmp = "levels\\";
	tmp.append(name);
	tmp.append(".txt");

	stopMusic();

	changeState(PlayState::Instance());
	PlayState::Instance()->setLevel(tmp);
	PlayState::Instance()->setLevelType(CUSTOM);

	return false;
}

void CustomLevelState::removeItem(std::string itemName)
{
	itemName.erase(0, 7);
	itemName.erase(itemName.size() - 4, itemName.size());
	mCustomLevelMenu->removeItem(itemName);
}