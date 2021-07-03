
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "core/system.h"

class PhysicsSystem : public System
{
public:
	void Init();

	void Update_VersusMode(float& dt);
	
	void Update_MetroidVaniaMode(float& dt);
	
	
private:
	
};

#endif
