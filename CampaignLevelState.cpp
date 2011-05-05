#include "CampaignLevelState.h"
#include <Windows.h>
#include <direct.h>
#include <tchar.h>
#include <string>
#include <algorithm>
#include "CameraManager.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "Menu.h"
#include "TestState.h"


class Menu;

extern CameraManager* gCameraManager;
extern Mouse* gMouse;

CampaignLevelState CampaignLevelState::mCampaignLevelState;

void CampaignLevelState::init(Game* game)
{
	// important!
	setGame(game);

	// create the menu
	//mCampaignLevelMenu = new Menu("CustemLevelMenu", MOUSE, true, 4, 4);
	mCampaignLevelMenu = new Menu("CustemLevelMenu", MOUSE, VER, 2, true, 4, 4);
	mCampaignLevelMenu->setMenuBackground("misc\\textures\\menu_bkgd.bmp", 700, 450, 256, 350);

	// get the files in the map folder
	std::vector<string> levelList = getLevels();

	// load the campaign progress
	mProgress.loadProgress("levels\\campaign\\campaign_progress.txt");

	// add menu items, each level
	for(int i = 0; i < levelList.size(); i++)
	{
		// remove .txt
		if(levelList[i].find(".txt") != string::npos || levelList[i].find(".TXT") != string::npos)
			levelList[i].erase(levelList[i].end()-4,levelList[i].end()); 

		if(mProgress.getProgress(levelList[i]).playable == 0)	{
			mCampaignLevelMenu->addMenuItem(levelList[i], (char*)GRAY_BUTTON_NORMAL_SOURCE.c_str(), (char*)GRAY_BUTTON_HOOVER_SOURCE.c_str());
			mCampaignLevelMenu->setPressable(levelList[i], false);
		}
		else
			mCampaignLevelMenu->addMenuItem(levelList[i], "misc\\textures\\level_menu_normal.bmp", "misc\\textures\\level_menu_hoover.bmp");
	}

	// build the menu
	mCampaignLevelMenu->buildMenu2();
	mCampaignLevelMenu->connect(&CampaignLevelState::menuHandler, this);
}

void CampaignLevelState::cleanup()
{
	delete mCampaignLevelMenu;
}

void CampaignLevelState::pause()
{
	// don't know how to to yet
}

void CampaignLevelState::resume()
{
	// don't know how to to yet
}

void CampaignLevelState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				changeState(MainMenuState::Instance());
		break;
	}
}

void CampaignLevelState::update(double dt)
{
	// call menuHandler() when a item is pressed
	// state changing takes place there
	updateMenu();
}

// just so CampaignEditorState can reach this
void CampaignLevelState::updateMenu(void)
{
	// consider to combine this with the menuHandler()
	mCampaignLevelMenu->update(gMouse->getPos());
}

void CampaignLevelState::drawMain(void)
{
	gGraphics->BlitRect(600, 450, 1200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
	mCampaignLevelMenu->draw();
}

void CampaignLevelState::drawGui(void)
{
	// the green side
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
}

std::vector<string> CampaignLevelState::getLevels(void)
{
	int size;
	int counter = 0;
	bool working(true);
	string test;
	char buffer[256] = "";
	std::vector<string> levelList;

	WIN32_FIND_DATA findData;
	HANDLE myHandle = FindFirstFile("levels\\campaign\\*",&findData); // C:\\hlserver\\*

	if (myHandle == INVALID_HANDLE_VALUE) 
	{
		 MessageBox(0, "Error loading the first file", 0, 0);
	} 
	else 
	{
		do
		{
			test = string(findData.cFileName);

			if (test != "."  && test !=  ".." && test != "campaign_progress.txt")
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

bool CampaignLevelState::menuHandler(std::string name)
{	
	//add levels\\ and .txt 
	string tmp = "levels\\campaign\\";
	tmp.append(name);
	tmp.append(".txt");

	changeState(PlayState::Instance());
	PlayState::Instance()->setLevel(tmp);

	return false;
}