#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include <vector>
#include "misc/camera.h"

class CameraSystem : public System
{
public:
	void Init(CustomCamera* camera_ptr,
				std::uint16_t screenWidth, std::uint16_t screenHeight);

	void Update();
	
protected:

	CustomCamera* m_camera_ptr;
};

#endif
