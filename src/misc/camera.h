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
		camera_rect = {0,0,game_res_width,game_res_height};
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
	
private:

	Rectangle camera_rect;
	std::uint16_t m_level_bound_left_x;
	std::uint16_t m_level_bound_right_x;
	std::uint16_t m_level_bound_up_y;
	std::uint16_t m_level_bound_down_y;
	
};


#endif
