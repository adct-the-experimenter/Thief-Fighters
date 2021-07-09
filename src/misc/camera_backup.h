#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include "raylib.h"

#include <bitset>

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

//screen
struct Screen
{
	Rectangle screen_rect;
	bool in_active_use;
};

//class to manager 4 cameras
class CameraManager
{
	
public:

	//indicates which cameras are split and active.
	enum class CS_State : std::uint8_t { NONE=0,
										//one player screen state
										S0_only,
										
									    //two player screen states
									    splitS0S1_joinNone, splitNone_joinS0S1,
									    
									    //three player screen states
									    splitS0S1S2_joinNone,
									    splitS0_joinS1S2, splitS1_joinS0S2, splitS2_joinS0S1,
									    splitNone_joinS0S1S2,
									    
									    //four player screen states
									    splitS0S1S2S3_joinNone,
									    splitS0_joinS1S2S3, splitS1_joinS0S2S3, splitS2_joinS0S1S3, splitS3_joinS0S1S2,
									    splitS0S1_joinS2S3, splitS0S2_joinS1S3,
									    splitNone_joinS0S1S2S3
									    
									    };
	
	CS_State camera_screen_state;
	
	std::uint8_t m_num_players;
	
	//cameras following players who have camera lead role
	std::array <CustomCamera,4> lead_cameras;
	
	//screens that show the cameras
	std::array <Screen,4> screens;
	
	float game_screen_width;
	float game_screen_height;
	
	//functions for split-screen manipulation
	
	//initialize for 1 screen game
	void SetForOneScreen(float game_res_width,float game_res_height)
	{
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width,game_res_height};
		(*lead_cameras[0].GetCameraRectPointer()) = {0,0,game_res_width,game_res_height};
		lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = false;
		lead_cameras[1].SetCameraActiveStatus(false);
		
		screens[2].in_active_use = false;
		lead_cameras[1].SetCameraActiveStatus(false);
		
		screens[3].in_active_use = false;
		lead_cameras[3].SetCameraActiveStatus(false);
		
		camera_screen_state = CS_State::S0_only;
	}
	
	//initialize for 2 screen game
	void SetForTwoScreens(float game_res_width,float game_res_height)
	{
		//split in half. width is halved. height is not changed.
		
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width / 2, game_res_height};
		(*lead_cameras[0].GetCameraRectPointer()) = {0,0,game_res_width / 2,game_res_height};
		lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = true;
		screens[1].screen_rect = {game_res_width / 2, 0,game_res_width / 2, game_res_height};
		(*lead_cameras[1].GetCameraRectPointer()) = {0,0,game_res_width / 2,game_res_height};
		lead_cameras[1].SetCameraActiveStatus(true);
		
		screens[2].in_active_use = false;
		lead_cameras[2].SetCameraActiveStatus(false);
		
		screens[3].in_active_use = false;
		lead_cameras[3].SetCameraActiveStatus(false);
		
		camera_screen_state = CS_State::splitS0S1_joinNone;
	}
	
	//initialize for 3 screen game
	void SetForThreeScreens(float game_res_width,float game_res_height)
	{
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[0].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = true;
		screens[1].screen_rect = {game_res_width / 2, 0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[1].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		lead_cameras[1].SetCameraActiveStatus(true);
		
		screens[2].in_active_use = true;
		screens[2].screen_rect = {0, game_res_height / 2,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[2].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		lead_cameras[2].SetCameraActiveStatus(true);
		
		screens[3].in_active_use = false;
		lead_cameras[3].SetCameraActiveStatus(false);
		
		camera_screen_state = CS_State::splitS0S1S2_joinNone;
	}
	
	//initialize for 4 screen game
	void SetForFourScreens(float game_res_width,float game_res_height)
	{
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[0].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = true;
		screens[1].screen_rect = {game_res_width / 2, 0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[1].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		lead_cameras[1].SetCameraActiveStatus(true);
		
		screens[2].in_active_use = true;
		screens[2].screen_rect = {0, game_res_height / 2,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[2].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		lead_cameras[2].SetCameraActiveStatus(true);
		
		screens[3].in_active_use = true;
		screens[3].screen_rect = {game_res_width / 2, game_res_height / 2,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[3].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		lead_cameras[3].SetCameraActiveStatus(true);
		
		camera_screen_state = CS_State::splitS0S1S2S3_joinNone;
	}
	
	//function to join screen zero and screen one
	void JoinScreenZeroAndScreenOne()
	{
		//assuming width and height of cameras have already been initialized
		//join 2 screens, height stays the same, width changes
		
		//make screen 0 and camera 0 bigger
		screens[0].in_active_use = true;
		screens[0].screen_rect.width = game_screen_width;
		lead_cameras[0].GetCameraRectPointer()->width = game_screen_width;;
		
		lead_cameras[0].SetCameraActiveStatus(true);
		
		//disable screen 1 and camera 1
		screens[1].in_active_use = false;
		screens[1].screen_rect.width = game_screen_width;
		lead_cameras[1].GetCameraRectPointer()->width = game_screen_width;
		
		lead_cameras[1].SetCameraActiveStatus(false);
	}
	
	//function to split screen zero and screen one
	void SplitScreenZeroAndScreenOne()
	{
		//split into 2 screens in half, height stays the same, width changes
		
		screens[0].in_active_use = true;
		screens[0].screen_rect.x = 0;
		screens[0].screen_rect.width = game_screen_width / 2;
		lead_cameras[0].GetCameraRectPointer()->width = game_screen_width / 2;

		lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = true;
		screens[1].screen_rect.x = game_screen_width / 2;
		screens[1].screen_rect.width = game_screen_width / 2;
		lead_cameras[1].GetCameraRectPointer()->width = game_screen_width / 2;
		
		lead_cameras[1].SetCameraActiveStatus(true);
	}
	
	//function to join screen two and screen three
	void JoinScreenTwoAndScreenThree()
	{
		//assuming width and height of cameras have already been initialized
		//join 2 screens, height stays the same, width changes
		
		screens[2].in_active_use = true;
		screens[2].screen_rect.width = game_screen_width;
		lead_cameras[2].GetCameraRectPointer()->width = game_screen_width;
		
		lead_cameras[2].SetCameraActiveStatus(true);
		
		//disable screen 1 and camera 1
		screens[3].in_active_use = false;
		screens[3].screen_rect.width = game_screen_width;
		lead_cameras[3].GetCameraRectPointer()->width = game_screen_width;
		
		lead_cameras[3].SetCameraActiveStatus(false);
	}
	
	//function to split screen two and screen three
	void SplitScreenTwoAndScreenThree()
	{
		//split into 2 screens in half, height stays the same, width changes
		
		screens[2].in_active_use = true;
		screens[2].screen_rect.x = 0;
		screens[2].screen_rect.width = game_screen_width / 2;
		lead_cameras[2].GetCameraRectPointer()->width = game_screen_width / 2;

		lead_cameras[2].SetCameraActiveStatus(true);
		
		//enable screen 3 and make it the same size as screen 2
		screens[3].in_active_use = true;
		screens[3].screen_rect.x = game_screen_width / 2;
		screens[3].screen_rect.width = game_screen_width / 2;
		lead_cameras[3].GetCameraRectPointer()->width = game_screen_width / 2;
		
		lead_cameras[3].SetCameraActiveStatus(true);
	}
	
	//function to join screen zero and two
	void JoinScreenZeroAndScreenTwo()
	{
		//assuming width and height of cameras have already been initialized
		//join 2 screens, width stays the same, height changes
		
		screens[0].in_active_use = true;
		screens[0].screen_rect.height = game_screen_height;
		lead_cameras[0].GetCameraRectPointer()->height = game_screen_height;
		
		lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[2].in_active_use = false;
		screens[2].screen_rect.height = game_screen_height;
		lead_cameras[2].GetCameraRectPointer()->height = game_screen_height;
		
		lead_cameras[2].SetCameraActiveStatus(false);
	}
	
	//function to split screen zero and screen two
	void SplitScreenZeroAndScreenTwo()
	{
		//split into 2 screens in half, width stays the same, height changes
		
		screens[0].in_active_use = true;
		screens[0].screen_rect.y = 0;
		screens[0].screen_rect.height = game_screen_height / 2;
		lead_cameras[0].GetCameraRectPointer()->height = game_screen_height / 2;

		lead_cameras[0].SetCameraActiveStatus(true);
		
		//enable screen 3 and make it the same size as screen 2
		screens[2].in_active_use = true;
		screens[2].screen_rect.y = game_screen_height / 2;
		screens[2].screen_rect.height = game_screen_height / 2;
		lead_cameras[2].GetCameraRectPointer()->width = game_screen_width / 2;
		lead_cameras[2].GetCameraRectPointer()->height = game_screen_height / 2;
		
		lead_cameras[2].SetCameraActiveStatus(true);
	}
	
	//function to join screen one and three
	void JoinScreenOneAndScreenThree()
	{
		//assuming width and height of cameras have already been initialized
		//join 2 screens, width stays the same, height changes
		
		screens[1].in_active_use = true;
		screens[1].screen_rect.height = game_screen_height;
		lead_cameras[1].GetCameraRectPointer()->height = game_screen_height;
		
		lead_cameras[1].SetCameraActiveStatus(true);
		
		screens[3].in_active_use = false;
		screens[3].screen_rect.height = game_screen_height;
		lead_cameras[3].GetCameraRectPointer()->height = game_screen_height;
		
		lead_cameras[3].SetCameraActiveStatus(false);
	}
	
	//function to split screen one and screen three
	void SplitScreenOneAndScreenThree()
	{
		//split into 2 screens in half, width stays the same, height changes
		
		screens[1].in_active_use = true;
		screens[1].screen_rect.y = 0;
		screens[1].screen_rect.height = game_screen_height / 2;
		lead_cameras[1].GetCameraRectPointer()->height = game_screen_height / 2;

		lead_cameras[1].SetCameraActiveStatus(true);
		
		//enable screen 3 and make it the same size as screen 2
		screens[3].in_active_use = true;
		screens[3].screen_rect.y = game_screen_height / 2;
		screens[3].screen_rect.height = game_screen_height / 2;
		lead_cameras[3].GetCameraRectPointer()->height = game_screen_height / 2;
		
		lead_cameras[3].SetCameraActiveStatus(true);
	}

private:
	
};

#endif
