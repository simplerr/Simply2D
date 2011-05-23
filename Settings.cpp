#include "Settings.h"
#include <fstream>

Settings::Settings(std::string source)
{
	mSource = source;
	loadSettings();
}

Settings::~Settings()
{

}

void Settings::loadSettings(void)
{
	std::ifstream fin;
	fin.open(mSource);
	
	// tmp is the rubric of a setting (ex. "music_muted")
	std::string tmp, musicMuted, effectsMuted;
	fin >> tmp >> musicMuted >> tmp >> effectsMuted >> tmp >> mMusicVolume;

	if(musicMuted == "1")
		mMusicMuted = true;
	else if(musicMuted == "0")
		mMusicMuted = false;

	if(effectsMuted == "1")
		mEffectsMuted = true;
	else if(effectsMuted == "0")
		mEffectsMuted = false;

	fin.close();
}
	
void Settings::saveSettings(void)
{
	std::ofstream fout;
	fout.open(mSource);

	fout << "music_muted " << mMusicMuted << std::endl;
	fout << "effects_muted " << mEffectsMuted << std::endl;
	fout << "music_volume " << mMusicVolume << std::endl;

	fout.close();
}

bool Settings::musicMuted(void)
{
	return mMusicMuted;
}

bool Settings::effectsMuted(void)
{
	return mEffectsMuted;
}

void Settings::muteMusic(bool b)
{
	mMusicMuted = b;
}
	
void Settings::muteEffects(bool b)
{
	mEffectsMuted = b;
}

void Settings::setMusicVolume(float volume)
{
	mMusicVolume = volume;
}

float Settings::musicVolume(void)
{
	return mMusicVolume;
}
