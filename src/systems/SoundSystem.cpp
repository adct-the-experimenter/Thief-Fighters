#include "SoundSystem.h"


#include "core/coordinator.h"

#include "misc/sound_media.h"

#include "misc/char_media.h"


extern Coordinator gCoordinator;

bool SoundSystem::Init()
{
	sound_system_initialized = true;
	
	LoadGeneralAudio();
    
	return true;
}

void SoundSystem::Update_VersusMode()
{
	
	//for every entity
	for (auto const& entity : mEntities)
	{
		auto& sound_comp = gCoordinator.GetComponent<SoundComponent>(entity);
		
		//play sound based on sound component information
		if(sound_comp.sound_type == SoundType::GENERAL_SOUND)
		{
			PlayGeneralSound(sound_comp.general_sound_id);
		}
		else if(sound_comp.sound_type == SoundType::CHAR_SOUND)
		{
			PlayCharacterSound(sound_comp.char_index,sound_comp.char_sound_id);
		}
		
		sound_comp.sound_type = SoundType::NONE;
		
	}
	
    
}
	
void SoundSystem::Update_MetroidVaniaMode()
{
	//play sound based on sound id received
	
}

void SoundSystem::Close()
{
	UnloadGeneralAudio();
}
