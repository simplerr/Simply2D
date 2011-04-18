#include "LevelCompletedState.h"
#include "CameraManager.h"
#include "MainMenuState.h"
#include "TestState.h"
#include "WindowHandler.h"

extern CameraManager* gCameraManager;
extern Mouse*		  gMouse;


LevelCompletedState LevelCompletedState::mLevelCompletedState;

void LevelCompletedState::init(Game* game)
{
	// important!
	setGame(game);

	mWindowHandler = new WindowHandler(700, 450, 600, 400);
	mWindowHandler->setBackground(LEVEL_COMPLETED_SOURCE);

	mCampaignNextButton = new Button(mWindowHandler, BUTTON_NEXTLEVEL, "Next", 500, 350, 100, 45, true);
	mCampaignNextButton->connect(&LevelCompletedState::messageHandler, this);

	mCustomNextButton = new Button(mWindowHandler, BUTTON_NEXTLEVEL, "Next", 500, 350, 100, 45, true, (char*)GRAY_BUTTON_NORMAL_SOURCE.c_str(), (char*)GRAY_BUTTON_HOOVER_SOURCE.c_str());

	mAgainButton = new Button(mWindowHandler, BUTTON_AGAIN, "Again", 100, 350, 100, 45, true);
	mAgainButton->connect(&LevelCompletedState::messageHandler, this);

	mMenuButton = new Button(mWindowHandler, BUTTON_MENU, "Menu", 300, 350, 100, 45, true);
	mMenuButton->connect(&LevelCompletedState::messageHandler, this);

	// load the background
	mBkgdTexture = gGraphics->loadTexture("misc\\textures\\city_bkgd_yellow.bmp"); 
		
	if(!mBkgdTexture)
		MessageBox(0, "Couldn't background texture", 0, 0);
}

void LevelCompletedState::cleanup()
{
	ReleaseCOM(mBkgdTexture);
	delete mWindowHandler;
}

void LevelCompletedState::pause()
{
	// don't know how to to yet
}

void LevelCompletedState::resume()
{
	// don't know how to to yet
}

void LevelCompletedState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_KEYDOWN:			
		if(wParam == VK_ESCAPE)
			changeState(MainMenuState::Instance());
		break;
	}
}

void LevelCompletedState::update(double dt)
{
	if(gDInput->mouseButtonPressed(LEFTBUTTON))	{
		//mWindow->sendMousePress(gMouse->getScreenPos().x, gMouse->getScreenPos().y);
	}

	mWindowHandler->update(dt);
}

void LevelCompletedState::drawMain(void)
{
	drawBkgd();
	mWindowHandler->draw();
}

void LevelCompletedState::drawGui(void)
{
	// green color
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB(155, 155, 200, 000));
}

void LevelCompletedState::drawBkgd()
{	
	RECT r1;
	r1.top = 0;
	r1.left = 0;
	r1.right = 4800;
	r1.bottom = 900;

	// scale 4x in x
	D3DXMATRIX texScaling;
	D3DXMatrixScaling(&texScaling, 4.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));

	gGraphics->BlitTexture(mBkgdTexture, r1, 0xFFFFFFFF, 0.0f);

	// restore to std scaling
	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 0.0f);
	HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &texScaling));
}

bool LevelCompletedState::messageHandler(WindowID id, string data)
{
	switch(id)
	{
	case BUTTON_NEXTLEVEL:
		{
			changeState(PlayState::Instance());
			string tmp = "levels\\";
			tmp.append(mNextLevel);
			tmp.append(".txt");
			PlayState::Instance()->setLevel(tmp);
			return false;
		}
	case BUTTON_MENU:
		{
			changeState(MainMenuState::Instance());
			return false;
		}
	case BUTTON_AGAIN:
		{
			changeState(PlayState::Instance());
			PlayState::Instance()->setLevel(mCurrentLevel);
			PlayState::Instance()->setLevelType(mCompletedType);
			return false;
		}
	}
	return true;
}

void LevelCompletedState::setCompletedType(LevelType type)
{
	mCompletedType = type;

	if(mCompletedType == CAMPAIGN)
		mCustomNextButton->setVisible(false);
	else if(mCompletedType == CUSTOM)
		mCampaignNextButton->setVisible(false);
}

void LevelCompletedState::setLevels(string current, string next)
{
	mCurrentLevel = current;
	mNextLevel = next;
}