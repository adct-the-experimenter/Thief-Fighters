#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include <vector>
#include "misc/camera.h"


class RenderSystem : public System
{
public:
	void Init(CustomCamera* camera);

	void Update();
		
private:

	CustomCamera* m_camera_ptr;
	
};

#endif
