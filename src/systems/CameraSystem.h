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
	
	void Init_MetroidVaniaMode(CameraManager* camera_ptr, std::uint8_t num_players,
								std::uint32_t gameScreenWidth, std::uint32_t gameScreenHeight);
	
	//updates scale according to calculations
	void Update_MetroidVaniaMode();
	
	void update_timer(float& dt);
	
protected:

	CustomCamera* m_camera_ptr;
	std::uint8_t m_num_players;
	
	float zoom_time_counter;
	
	CameraManager* m_camera_manager_ptr;
	
	std::uint16_t level_bound_left_x;
	std::uint16_t level_bound_right_x;
	std::uint16_t level_bound_up_y;
	std::uint16_t level_bound_down_y;
};

#endif
