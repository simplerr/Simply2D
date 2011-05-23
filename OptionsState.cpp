#include "OptionsState.h"
#include "MainMenuState.h"
#include "WindowHandler.h"
#include "CheckBox.h"
#include "Sound.h"
#include "Settings.h"
#include "Slidebar.h"

extern Sound *gSound;
OptionsState OptionsState::mOptionsState;

void OptionsState::init(Game* game)
{
	// important!
	setGame(game);

	mWindowHandler = new WindowHandler(700, 500, 300, 400);
	mWindowHandler->setBackground("misc\\textures\\menu_bkgd.bmp");

	mMuteMusic = new CheckBox(mWindowHandler, MUTE_MUSIC, "Music:", 145, 50);
	mMuteMusic->connect(&OptionsState::messageHandler, this);
	mMuteEffects = new CheckBox(mWindowHandler, MUTE_EFFECTS, "Effects: ", 145, 80);
	mMuteEffects->connect(&OptionsState::messageHandler, this);

	mSlidebar = new Slidebar(mWindowHandler, SOUND_SLIDER, "Music volume:", 125, 140, 300, 20, 1, 0, 1);
	mSlidebar->connect(&OptionsState::messageHandler, this);

	if(gSound->getMusicMuted())
		mMuteMusic->setChecked(true);
	else if(!gSound->getMusicMuted())
		mMuteMusic->setChecked(false);

	if(gSound->getEffectsMuted())
		mMuteEffects->setChecked(true);
	else if(!gSound->getEffectsMuted())
		mMuteEffects->setChecked(false);

	// load the settings
	mSettings = new Settings("config.txt");

	mSlidebar->setValue(mSettings->musicVolume());
}
	
void OptionsState::cleanup()
{
	delete mWindowHandler;
	delete mSettings;
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

bool OptionsState::messageHandler(WindowID id, WindowMessage msg)
{
	switch(id)
	{
	case MUTE_MUSIC:
		{
			if(mMuteMusic->getChecked())	{
				gSound->muteMusic(true);
				mSettings->muteMusic(true);
			}
			else if(!mMuteMusic->getChecked())	{
				gSound->muteMusic(false);
				mSettings->muteMusic(false);
				gSound->playMusic("misc\\sound\\menu_loop.wav", true, true);
			}

			mSettings->saveSettings();
			return false;
		}
	case MUTE_EFFECTS:
		{
			if(mMuteEffects->getChecked())	{
				gSound->muteEffects(true);
				mSettings->muteEffects(true);
			}
			else if(!mMuteEffects->getChecked())	{
				gSound->muteEffects(false);
				mSettings->muteEffects(false);
			}
			mSettings->saveSettings();
			return false;
		}
	case SOUND_SLIDER:
		{
			gSound->setVolume(msg.getFloat());
			mSettings->setMusicVolume(msg.getFloat());
			mSettings->saveSettings();
			return false;
		}
	}
	

	return true;
}