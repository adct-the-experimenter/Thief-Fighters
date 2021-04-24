#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "core/system.h"

class AnimationSystem : public System
{
public:
	void Init();

	void Update(float& dt);
	
private:
	
	//counter used to keep track of time based on time since last frame was drawn
	float m_time_counter;
	
};

#endif

