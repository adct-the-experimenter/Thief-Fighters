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
	void Init(CustomCamera* camera_ptr, std::uint8_t num_players);
	
	//updates scale according to calculations
	void Update(float& scale);
	
protected:

	CustomCamera* m_camera_ptr;
	std::uint8_t m_num_players;
	
	
};

#endif
