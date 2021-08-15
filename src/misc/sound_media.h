#ifndef SOUND_MEDIA_H
#define SOUND_MEDIA_H

#include "raylib.h"

#include "general_sounds.h"
#include "char_sounds.h"

//extern Sound hit_sound;
extern Sound general_sounds[5];


//function to load general audio
bool LoadGeneralAudio();

//function to play general sound based on general sound id
void PlayGeneralSound(GeneralSoundID& sound_id);

//function to play character sound based on character sound id and character
void PlayCharacterSound(std::int16_t& char_index, CharSoundID& sound_id);

//function to unload general audio
void UnloadGeneralAudio();

#endif
