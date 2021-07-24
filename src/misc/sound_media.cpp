#include "sound_media.h"

#include "globalvariables.h"

#include "char_media.h"

//General sounds
Sound general_sounds[5];


bool LoadGeneralAudio()
{
	std::string test_audio_file = DATADIR_STR + "/foom_0.wav";
	
	general_sounds[0] = LoadSound(test_audio_file.c_str());
	
	return true;
}

void PlayGeneralSound(GeneralSoundID& sound_id)
{
	PlaySound(general_sounds[ static_cast<int>(sound_id) ]);
}

void PlayCharacterSound(std::uint8_t& char_index,CharSoundID& sound_id)
{
	PlaySound(character_sounds[char_index].sounds[ static_cast<int>(sound_id) ]);
}

void UnloadGeneralAudio()
{
	UnloadSound(general_sounds[0]);
}
