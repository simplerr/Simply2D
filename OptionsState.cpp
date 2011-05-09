#include "OptionsState.h"
#include "MainMenuState.h"
#include "WindowHandler.h"
#include "CheckBox.h"
#include "Sound.h"

extern Sound *gSound;
OptionsState OptionsState::mOptionsState;

void OptionsState::init(Game* game)
{
	// important!
	setGame(game);

	mWindowHandler = new WindowHandler(700, 500, 200, 400);
	mWindowHandler->setBackground("misc\\textures\\menu_bkgd.bmp");

	mMuteMusic = new CheckBox(mWindowHandler, MUTE_MUSIC, "Music:", 145, 50);
	mMuteMusic->connect(&OptionsState::messageHandler, this);
	mMuteEffects = new CheckBox(mWindowHandler, MUTE_EFFECTS, "Effects: ", 145, 80);
	mMuteEffects->connect(&OptionsState::messageHandler, this);

	if(gSound->getMusicMuted())
		mMuteMusic->setValue("True");
	else if(!gSound->getMusicMuted())
		mMuteMusic->setValue("False");

	if(gSound->getEffectsMuted())
		mMuteEffects->setValue("True");
	else if(!gSound->getEffectsMuted())
		mMuteEffects->setValue("False");
}
	
void OptionsState::cleanup()
{

}
	
void OptionsState::pause()
{

}
	
void OptionsState::resume()
{

}
	
void OptionsState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_KEYDOWN:			
		if(wParam == VK_ESCAPE)	{
			changeState(MainMenuState::Instance());
		}
		break;
	}
}
	
void OptionsState::update(double dt)
{
	mWindowHandler->update(dt);
}
	
void OptionsState::drawMain(void)
{
	// green bkgd
	gGraphics->BlitRect(600, 450, 1200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
	
	// windows
	mWindowHandler->draw();
}

void OptionsState::drawGui(void)
{
	// the green side
	gGraphics->BlitRect(1300, 450, 200, 900, D3DCOLOR_ARGB( 155, 155, 200, 000));
}

bool OptionsState::messageHandler(WindowID id, string data)
{
	strcpy(buffer, data.c_str());

	switch(id)
	{
	case MUTE_MUSIC:
		{
			if(strcmp(buffer, "True") == 0)
				gSound->muteMusic(true);
			else if(strcmp(buffer, "False") == 0)	{
				gSound->muteMusic(false);
				gSound->playMusic("misc\\sound\\menu_loop.wav", true, true);
			}
			return false;
		}
	case MUTE_EFFECTS:
		{
			if(strcmp(buffer, "True") == 0)
				gSound->muteEffects(true);
			else if(strcmp(buffer, "False") == 0)
				gSound->muteEffects(false);
			return false;
		}
	}

	return true;
}