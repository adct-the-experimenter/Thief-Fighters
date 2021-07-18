#include "sound_media.h"

#include "globalvariables.h"

Sound hit_sound;

bool LoadGeneralAudio()
{
	std::string test_audio_file = DATADIR_STR + "/foom_0.wav";
	
	hit_sound = LoadSound(test_audio_file.c_str());
	
	return true;
}

void UnloadGeneralAudio()
{
	UnloadSound(hit_sound);
}
