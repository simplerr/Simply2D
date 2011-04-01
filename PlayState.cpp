#include "PlayState.h"
#include "CameraManager.h"
#include "MainMenuState.h"

extern CameraManager* gCameraManager;

PlayState PlayState::mPlayState;

void PlayState::init(Game* game)
{
	// important!
	setGame(game);

	mPlayer = new Player("misc\\textures\\player.bmp", USER_WIDTH, USER_HEIGHT);
	mLevel = new Level(mPlayer);

	// load the background
	mBkgdTexture = gGraphics->loadTexture("misc\\textures\\city_bkgd_yellow.bmp"); 
		
	if(!mBkgdTexture)
		MessageBox(0, "Couldn't background texture", 0, 0);
}

void PlayState::cleanup()
{
	delete mPlayer;
	delete mLevel;

	ReleaseCOM(mBkgdTexture);
}

void PlayState::pause()
{
	// don't know how to to yet
}

void PlayState::resume()
{
	// don't know how to to yet
}

void PlayState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				changeState(MainMenuState::Instance());
		break;
	}
}

void PlayState::update(double dt)
{
	mLevel->updateLevel(dt);	// update objects + player
	gCameraManager->gameCamera()->move();		// move the camera accordingly
}

void PlayState::drawMain(void)
{
	drawBkgd();
	mLevel->drawLevel();
}

void PlayState::drawGui(void)
{
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
}

void PlayState::drawBkgd()
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

void PlayState::setLevel(string level)
{
	string levelToLoad = "levels\\";
	levelToLoad.append(level);
	mLevel->loadLevel((char*)levelToLoad.c_str()); // (char*)level.c_str()
}