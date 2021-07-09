#ifndef MULTIPLE_RENDER_SYSTEM_H
#define MULTIPLE_RENDER_SYSTEM_H

#include "systems/RenderSystem.h"

class MultipleRenderComponentSystem : public RenderSystem
{
public:
	void Init(std::array <CustomCamera,4> *cameras);

	void Update();
	
private:
	
};

#endif
