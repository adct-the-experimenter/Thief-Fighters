#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include "raylib.h"

class CustomCamera
{
	
public:

	void Init(	std::uint16_t game_res_width,
				std::uint16_t game_res_height)
	{
		camera_rect = {0,0,static_cast<float>(game_res_width),static_cast<float>(game_res_height)};
	}
	
	void SetLevelBounds(std::uint16_t level_bound_left_x,
				std::uint16_t level_bound_right_x,
				std::uint16_t level_bound_up_y,
				std::uint16_t level_bound_down_y)
	{
		m_level_bound_left_x = level_bound_left_x;
		m_level_bound_right_x = level_bound_right_x;
		m_level_bound_up_y = level_bound_up_y;
		m_level_bound_down_y = level_bound_down_y;
	}
	
	std::uint16_t& GetLeftLevelBound()
	{
		return m_level_bound_left_x;
	}
	
	std::uint16_t& GetRightLevelBound()
	{
		return m_level_bound_right_x;
	}
	
	std::uint16_t& GetUpLevelBound()
	{
		return m_level_bound_up_y;
	}
	
	std::uint16_t& GetDownLevelBound()
	{
		return m_level_bound_down_y;
	}
	
	Rectangle* GetCameraRectPointer()
	{
		return &camera_rect;
	}
	
	void SetCameraLeadPlayerNumber(std::uint8_t player_num)
	{
		m_camera_lead_player_num = player_num;
	}
	
	std::uint8_t GetCameraLeadPlayerNumber()
	{
		return m_camera_lead_player_num;
	}
	
	void SetCameraActiveStatus(bool state)
	{
		in_active_use = state;
	}
	
	bool GetCameraActiveStatus()
	{
		return in_active_use;
	}
	
	void SetWorldID(std::uint8_t id)
	{
		m_world_id = id;
	}
	
	std::uint8_t GetWorldID()
	{
		return m_world_id;
	}
	
private:

	Rectangle camera_rect;
	std::uint16_t m_level_bound_left_x;
	std::uint16_t m_level_bound_right_x;
	std::uint16_t m_level_bound_up_y;
	std::uint16_t m_level_bound_down_y;
	
	//camera lead
	//the player number who is camera lead i.e. player 1-8
	std::uint8_t m_camera_lead_player_num;
	
	//bool to indicate if camera is in active use
	bool in_active_use;
	
	//indicates which world camera is in
	std::uint8_t m_world_id;
};

//class to manager 4 cameras
class CameraManager
{
public:
	std::array <CustomCamera,4> lead_cameras;
	
};

#endif
