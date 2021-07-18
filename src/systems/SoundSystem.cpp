#include "SoundSystem.h"


#include "misc/sound_media.h"


bool SoundSystem::Init()
{
	sound_system_initialized = true;
	
	LoadGeneralAudio();
    
	return true;
}

void SoundSystem::Update_VersusMode()
{
	//play sound based on sound id received
	
	//PlaySound(hit_sound);
    
}
	
void SoundSystem::Update_MetroidVaniaMode()
{
	//play sound based on sound id received
	
}

void SoundSystem::Close()
{
	
}
