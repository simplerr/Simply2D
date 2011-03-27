#include "PlayState.h"
#include "CameraManager.h"
#include "MainMenuState.h"

extern CameraManager* gCameraManager;

PlayState PlayState::mPlayState;

void PlayState::init()
{
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

void PlayState::handleEvents(Game* game)
{
	if(gDInput->keyPressed(DIK_ESCAPE))
		game->changeState(MainMenuState::Instance());

}

void PlayState::update(Game* game, double dt)
{
	mLevel->updateLevel(dt);	// update objects + player
	gCameraManager->gameCamera()->move();		// move the camera accordingly
}

void PlayState::drawMain(Game* game)
{
	drawBkgd();
	mLevel->drawLevel();
}

void PlayState::drawGui(Game* game)
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