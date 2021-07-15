#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "core/miniaudio.h"

class SoundSystem
{

public:
	//intialize sound system
	bool Init();
	
	void Update_VersusMode();
	
	void Update_MetroidVaniaMode();
	
	//close sound system
	void Close();
	
private:
	//music buffers, 4 since there are a max of 4 screens.
	
	//sound effect buffers, 25 max sounds at at time on screen
	
	//sound bank
	
};

#endif
