#include "Sound.h"
#include "Settings.h"

Sound *gSound = 0;

Sound::Sound()
{
	mEngine = irrklang::createIrrKlangDevice();
	mMusic = NULL;
	mMusicSource = "none";
	// should read from a text file
	mMusicMuted = false;
	mEffectsMuted = false;

	// load the settings
	mSettings = new Settings("config.txt");

	if(mSettings->musicMuted())
		mMusicMuted = true;
	else if(!mSettings->musicMuted())
		mMusicMuted = false;

	if(mSettings->effectsMuted())
		mEffectsMuted = true;
	else if(!mSettings->effectsMuted())
		mEffectsMuted = false;

	mVolume = mSettings->musicVolume();
}
	
Sound::~Sound()
{
	if(mMusic != NULL)	{
		mMusic->stop();
		mMusic->drop();
	}
	mEngine->removeAllSoundSources();
	mEngine->drop(); 
	delete mSettings;
}

irrklang::ISound* Sound::playMusic(std::string source, bool loop, bool track)
{
	irrklang::ISound *music;
	if(!mMusicMuted)	
	{
		if(mMusicSource != source)
		{
			if(mMusic != NULL)	{
				mMusic->stop();
				mMusic->drop();
				mMusic = NULL;
			}

			mMusicSource = source;
			mMusic = mEngine->play2D(mMusicSource.c_str(), loop, false, true);
			mMusic->setVolume(mVolume);
		}
	}

	return NULL;
}
	
irrklang::ISound* Sound::playEffect(std::string source)
{
	//irrklang::ISound *effect;
	if(!mEffectsMuted)	{
		irrklang::ISound* soundEffect = mEngine->play2D(source.c_str(), false, false, true);
	}

	return NULL;
}

void Sound::muteMusic(bool mute)
{
	if(mute && mMusic != NULL)	{
		mMusic->stop();
		mMusic->drop();
		mMusic = NULL;
		mMusicSource = "none";
	}

	mMusicMuted = mute;
}
	
void Sound::muteEffects(bool mute)
{
	mEffectsMuted = mute;
}

bool Sound::getMusicMuted(void)
{
	return mMusicMuted;
}
	
bool Sound::getEffectsMuted(void)
{
	return mEffectsMuted;
}

void Sound::stopMusic(bool stop)
{
	if(stop)
		mMusic->stop();
}

void Sound::setVolume(float volume)
{
	mVolume = volume;
	if(mMusic != NULL)
		mMusic->setVolume(mVolume);
}