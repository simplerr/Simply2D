#ifndef SOUND_H
#define SOUND_H

#include <string>
#include "C:\Program Files (x86)\irrKlang-1.3.0\include\irrKlang.h"

#pragma comment(lib, "C:\\Program Files (x86)\\irrKlang-1.3.0\\lib\\Win32-visualStudio\\irrKlang.lib") // link with irrKlang.dll

class Settings;

class Sound
{
public:
	Sound();
	~Sound();

	irrklang::ISound* playMusic(std::string source, bool loop, bool track);
	irrklang::ISound* playEffect(std::string source);
	void stopMusic(bool stop);

	void muteMusic(bool mute);
	void muteEffects(bool mute);

	bool getMusicMuted(void);
	bool getEffectsMuted(void);
private:
	bool mMusicMuted;
	bool mEffectsMuted;
	irrklang::ISoundEngine* mEngine;
	irrklang::ISound* mMusic;
	std::string mMusicSource;
	Settings *mSettings;
};

#endif