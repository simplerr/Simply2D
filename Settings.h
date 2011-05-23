#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
public:
	Settings(std::string source);
	~Settings();

	void loadSettings(void);
	void saveSettings(void);

	bool musicMuted(void);
	bool effectsMuted(void);
	float musicVolume(void);


	void muteMusic(bool b);
	void muteEffects(bool b);
	void setMusicVolume(float volume);
private:
	std::string		mSource;
	bool			mMusicMuted;
	bool			mEffectsMuted;
	float			mMusicVolume;
};

#endif