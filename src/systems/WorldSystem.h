
#ifndef WORLD_SYSTEM_H
#define WORLD_SYSTEM_H

#include "core/system.h"

class WorldSystem : public System
{
public:
	void Init();
	
	void logic(float& dt);
	void render();
	
private:
	
};

#endif
