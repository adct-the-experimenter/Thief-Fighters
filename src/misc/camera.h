#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include "raylib.h"

#include <bitset>
#include <iostream>

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
	
	void SetLeadPlayerCoordinates(float x, float y)
	{
		lead_player_x = x;
		lead_player_y = y;
	}
	
	void GetLeadPlayerCoordinates(float& x, float& y)
	{
		x = lead_player_x;
		y = lead_player_y;
	}
	
	/*
	void SetCameraActiveStatus(bool state)
	{
		in_active_use = state;
	}
	
	bool GetCameraActiveStatus()
	{
		return in_active_use;
	}
	*/
	
	void SetWorldID(std::uint8_t id)
	{
		m_world_id = id;
	}
	
	std::uint8_t GetWorldID()
	{
		return m_world_id;
	}
	
private:

	//coordinates of player
	float lead_player_x;
	float lead_player_y;
	
	Rectangle camera_rect;
	std::uint16_t m_level_bound_left_x;
	std::uint16_t m_level_bound_right_x;
	std::uint16_t m_level_bound_up_y;
	std::uint16_t m_level_bound_down_y;
	
	//camera lead
	//the player number who is camera lead i.e. player 1-8
	std::uint8_t m_camera_lead_player_num;
	
	//bool to indicate if camera is in active use
	//bool in_active_use;
	
	//indicates which world camera is in
	std::uint8_t m_world_id;
};

//screen
struct Screen
{
	Rectangle screen_rect;
	bool in_active_use;
	CustomCamera* camera_ptr;
	Rectangle* camera_rect_ptr;
	Rectangle screen_camera;
};

//secondary camera
struct SecondaryCamera
{
	//camera to follow
	CustomCamera* camera_to_follow_ptr;
	
	//camera that is attached to screen
	CustomCamera camera_to_screen;
};

//class to manager 4 cameras
class CameraManager
{
	
public:

	//indicates which cameras are joined.
	//0 for split, 1 for join
	std::bitset <4> screens_joined_bitset;
	
	std::uint8_t m_num_players;
	
	
	//cameras following players who have camera lead role
	std::array <CustomCamera,4> lead_cameras;
	
	//screens that show the cameras
	std::array <Screen,4> screens;
	
	//camera used for when 3 screens are joined. Provides camera for small third screen.
	SecondaryCamera secondary_camera;
	
	float game_screen_width;
	float game_screen_height;
	
	//functions for split-screen manipulation
	
	//initialize for 1 screen game
	void SetForOneScreen(float game_res_width,float game_res_height)
	{
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width,game_res_height};
		(*lead_cameras[0].GetCameraRectPointer()) = {0,0,game_res_width,game_res_height};
		screens[0].screen_camera = {0,0,game_res_width,game_res_height};
		
		screens[1].in_active_use = false;
		
		screens[2].in_active_use = false;
		
		screens[3].in_active_use = false;
		
		//set as all split
		screens_joined_bitset[0] = 0;
		screens_joined_bitset[1] = 0;
		screens_joined_bitset[2] = 0;
		screens_joined_bitset[4] = 0;
		
		m_num_screens_used = 1;
	}
	
	//initialize for 2 screen game
	void SetForTwoScreens(float game_res_width,float game_res_height)
	{
		//split in half. width is halved. height is not changed.
		
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width / 2, game_res_height};
		(*lead_cameras[0].GetCameraRectPointer()) = {0,0,game_res_width / 2,game_res_height};
		//lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = true;
		screens[1].screen_rect = {game_res_width / 2, 0,game_res_width / 2, game_res_height};
		(*lead_cameras[1].GetCameraRectPointer()) = {0,0,game_res_width / 2,game_res_height};
		//lead_cameras[1].SetCameraActiveStatus(true);
		
		screens[2].in_active_use = false;
		//lead_cameras[2].SetCameraActiveStatus(false);
		
		screens[3].in_active_use = false;
		//lead_cameras[3].SetCameraActiveStatus(false);
		
		//set as all split
		screens_joined_bitset[0] = 0;
		screens_joined_bitset[1] = 0;
		screens_joined_bitset[2] = 0;
		screens_joined_bitset[4] = 0;
		
		m_num_screens_used = 2;
	}
	
	//initialize for 3 screen game
	void SetForThreeScreens(float game_res_width,float game_res_height)
	{
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[0].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		//lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = true;
		screens[1].screen_rect = {game_res_width / 2, 0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[1].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		//lead_cameras[1].SetCameraActiveStatus(true);
		
		screens[2].in_active_use = true;
		screens[2].screen_rect = {0, game_res_height / 2,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[2].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		//lead_cameras[2].SetCameraActiveStatus(true);
		
		screens[3].in_active_use = false;
		//lead_cameras[3].SetCameraActiveStatus(false);
		
		//set as all split
		screens_joined_bitset[0] = 0;
		screens_joined_bitset[1] = 0;
		screens_joined_bitset[2] = 0;
		screens_joined_bitset[4] = 0;
		
		m_num_screens_used = 3;
	}
	
	//initialize for 4 screen game
	void SetForFourScreens(float game_res_width,float game_res_height)
	{
		screens[0].in_active_use = true;
		screens[0].screen_rect = {0,0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[0].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		//lead_cameras[0].SetCameraActiveStatus(true);
		
		screens[1].in_active_use = true;
		screens[1].screen_rect = {game_res_width / 2, 0,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[1].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		//lead_cameras[1].SetCameraActiveStatus(true);
		
		screens[2].in_active_use = true;
		screens[2].screen_rect = {0, game_res_height / 2,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[2].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		//lead_cameras[2].SetCameraActiveStatus(true);
		
		screens[3].in_active_use = true;
		screens[3].screen_rect = {game_res_width / 2, game_res_height / 2,game_res_width / 2, game_res_height / 2};
		(*lead_cameras[3].GetCameraRectPointer()) = (Rectangle){0,0,game_res_width / 2,game_res_height / 2};
		//lead_cameras[3].SetCameraActiveStatus(true);
		
		//set as all split
		screens_joined_bitset[0] = 0;
		screens_joined_bitset[1] = 0;
		screens_joined_bitset[2] = 0;
		screens_joined_bitset[4] = 0;
		
		m_num_screens_used = 4;
	}
	
	void AttachCameraToScreen(CustomCamera* camera, std::uint8_t screen_num)
	{
		if(screen_num >= 4)
		{
			std::cout << "\nError out of range screen index used in AttachCameraToScreen!\n";
			return;
		}
		
		screens[screen_num].camera_ptr = camera;
		screens[screen_num].camera_rect_ptr = camera->GetCameraRectPointer();
	}
	
	//function to join screen zero and screen one
	void JoinScreenZeroAndScreenOne()
	{
		screens_joined_bitset.set(0);
		screens_joined_bitset.set(1);
		ApplyNewScreenState();		
	}
	
	//function to split screen zero and screen one
	void SplitScreenZeroAndScreenOne()
	{
		screens_joined_bitset.reset(0);
		screens_joined_bitset.reset(1);
		ApplyNewScreenState();
	}
	
	//function to join screen two and screen three
	void JoinScreenTwoAndScreenThree()
	{
		screens_joined_bitset.set(2);
		screens_joined_bitset.set(3);
		ApplyNewScreenState();
	}
	
	//function to split screen two and screen three
	void SplitScreenTwoAndScreenThree()
	{
		screens_joined_bitset.reset(2);
		screens_joined_bitset.reset(3);
		ApplyNewScreenState();
	}
	
	//function to join screen zero and two
	void JoinScreenZeroAndScreenTwo()
	{
		screens_joined_bitset.set(0);
		screens_joined_bitset.set(2);
		ApplyNewScreenState();
	}
	
	//function to split screen zero and screen two
	void SplitScreenZeroAndScreenTwo()
	{
		screens_joined_bitset.reset(0);
		screens_joined_bitset.reset(2);
		ApplyNewScreenState();
	}
	
	//function to join screen one and three
	void JoinScreenOneAndScreenThree()
	{
		screens_joined_bitset.set(1);
		screens_joined_bitset.set(3);
		ApplyNewScreenState();
	}
	
	//function to split screen one and screen three
	void SplitScreenOneAndScreenThree()
	{
		screens_joined_bitset.set(1);
		screens_joined_bitset.set(3);
		ApplyNewScreenState();
	}
	
	//function to update screen cameras based on lead player coordinates and screen camera dimensions
	void UpdateScreenCameraLocation()
	{
		float x,y;
		
		for(std::uint8_t i = 0; i < m_num_screens_used; i++)
		{
			
			screens[i].camera_ptr->GetLeadPlayerCoordinates(x,y);
			
			screens[i].screen_camera.x = x - 0.5*screens[i].screen_camera.width;
			
			if(screens[i].screen_camera.x < m_level_bound_left_x)
			{
				screens[i].screen_camera.x = m_level_bound_left_x;
			}
			
			if(screens[i].screen_camera.x + screens[i].screen_camera.width > m_level_bound_right_x)
			{
				screens[i].screen_camera.x = m_level_bound_right_x - screens[i].screen_camera.width;
			}
			
			screens[i].screen_camera.y = y - 0.5*screens[i].screen_camera.height;
			
			if(screens[i].screen_camera.y < m_level_bound_up_y)
			{
				screens[i].screen_camera.y = m_level_bound_up_y;
			}
			
			if(screens[i].screen_camera.y > m_level_bound_down_y + screens[i].screen_camera.height)
			{
				screens[i].screen_camera.y = m_level_bound_down_y + screens[i].screen_camera.height;
			}
		}
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
	
	void ApplyNewScreenState()
	{
		
		if(m_num_screens_used == 2)
		{
			//if screen zero and screen one not joined
			if(!screens_joined_bitset[0] && !screens_joined_bitset[1])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
								
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width,game_screen_height};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
			}
			//if screen zero and screen one joined
			else if(screens_joined_bitset[0] && screens_joined_bitset[1])
			{
				//enable screen 0 and its attached camera
				//make it bigger
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height};
				
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){game_screen_width,0,game_screen_width,game_screen_height};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height};
				
			}
		}
		else if(m_num_screens_used >= 3)
		{
			//if screen two, screen one, screen zero split
			//000
			if(!screens_joined_bitset[2] && !screens_joined_bitset[1] && !screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){game_screen_width / 4,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
			}
			//if screen two split, screen one and screen zero joined
			//011
			else if(!screens_joined_bitset[2] && screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				//stretch it to screen 1
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width ,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
			}
			//if screen two and screen zero joined, and screen one split
			//101
			else if(screens_joined_bitset[2] && !screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				//stretch it to screen 2
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				
				//disable screen 2 and its attached camera
				screens[2].in_active_use = false;
				screens[2].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height };
				
				
			}
			//if screen two and screen one joined, and screen zero split
			//110
			else if(screens_joined_bitset[2] && screens_joined_bitset[1] && !screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				
				//disable screen 2 and its attached camera
				screens[2].in_active_use = false;
				screens[2].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height };
			}
			//if all screens joined
			//111
			else if(screens_joined_bitset[2] && screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				//stretch it to screen 1 and screen 2
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height};
				
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height};
				
				
				//disable screen 2 and its attached camera
				screens[2].in_active_use = false;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2 ,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
								
			}
		
		}
		else if(m_num_screens_used == 4)
		{
			//if screen three, screen two, screen one, screen zero split
			//0000
			if(!screens_joined_bitset[3] && !screens_joined_bitset[2] && !screens_joined_bitset[1] && !screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 3 and its attached camera
				screens[3].in_active_use = true;
				screens[3].screen_rect = (Rectangle){game_screen_width / 2,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
			}
			//if screen three split and screen two split, screen one and screen zero joined
			//0011
			else if(!screens_joined_bitset[3] && !screens_joined_bitset[2] && screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				//stretch it to screen 1
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width ,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 3 and its attached camera
				screens[3].in_active_use = true;
				screens[3].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
			}
			//if screen two and screen zero joined, and screen one and screen three split
			//0101
			else if(!screens_joined_bitset[3] && screens_joined_bitset[2] && !screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				//stretch it to screen 2
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				
				//disable screen 2 and its attached camera
				screens[2].in_active_use = false;
				screens[2].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2 };
				
				//enable screen 3 and its attached camera
				screens[3].in_active_use = true;
				screens[3].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2 };
				
			}
			//if screen two and screen one joined, and screen three and screen zero split
			//0110
			else if(!screens_joined_bitset[3] && screens_joined_bitset[2] && screens_joined_bitset[1] && !screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				
				//disable screen 2 and its attached camera
				screens[2].in_active_use = false;
				screens[2].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 3 and its attached camera
				screens[3].in_active_use = true;
				screens[3].screen_rect = (Rectangle){game_screen_width / 2,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
			}
			//if screen zero and screen three joined, and screen two and screen one split
			//1001
			else if(screens_joined_bitset[3] && !screens_joined_bitset[2] && !screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//disable screen 3 and its attached camera
				screens[3].in_active_use = false;
				screens[3].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
			}
			//if screen three and screen two joined, and screen zero and screen one split
			else if(screens_joined_bitset[3] && screens_joined_bitset[2] && !screens_joined_bitset[1] && !screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//disable screen 3 and its attached camera
				screens[3].in_active_use = false;
				screens[3].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width ,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
			}
			//1100
			//if screen three split, screens zero one and two joined
			//0111
			else if(!screens_joined_bitset[3] && screens_joined_bitset[2] && screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				//disable screen 2 and its attached camera
				screens[2].in_active_use = false;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//enable screen 3 and its attached camera
				screens[3].in_active_use = true;
				screens[3].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width ,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
			}
			//if screen two split, screens zero one and three joined
			//1011
			else if(screens_joined_bitset[3] && !screens_joined_bitset[2] && screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height};
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				//disable screen 3 and its attached camera
				screens[3].in_active_use = false;
				screens[3].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
			}
			//if screen one split, screens zero two and three joined
			//1101
			else if(screens_joined_bitset[3] && screens_joined_bitset[2] && !screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//disable screen 0 and its attached camera
				screens[0].in_active_use = false;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				
				//enable screen 1 and its attached camera
				screens[1].in_active_use = true;
				screens[1].screen_rect = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				//disable screen 3 and its attached camera
				screens[3].in_active_use = false;
				screens[3].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
			}
			//if screen zero split, and screens one two and three joined
			//1110
			else if(screens_joined_bitset[3] && screens_joined_bitset[2] && screens_joined_bitset[1] && !screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width ,game_screen_height / 2};
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				//enable screen 2 and its attached camera
				screens[2].in_active_use = true;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height / 2};
				
				//disable screen 3 and its attached camera
				screens[3].in_active_use = false;
				screens[3].screen_rect = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
			}
			//if all screens joined
			//1111
			else if(screens_joined_bitset[3] && screens_joined_bitset[2] && screens_joined_bitset[1] && screens_joined_bitset[0])
			{
				//enable screen 0 and its attached camera
				//stretch it to screen 1 and screen 2
				screens[0].in_active_use = true;
				screens[0].screen_rect = (Rectangle){0,0,game_screen_width ,game_screen_height};
				screens[0].screen_camera = (Rectangle){0,0,game_screen_width,game_screen_height};
				
				
				//disable screen 1 and its attached camera
				screens[1].in_active_use = false;
				screens[1].screen_rect = (Rectangle){game_screen_width / 2,0,game_screen_width / 2,game_screen_height / 2};
				screens[1].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				
				//disable screen 2 and its attached camera
				screens[2].in_active_use = false;
				screens[2].screen_rect = (Rectangle){0,game_screen_height / 2,game_screen_width / 2 ,game_screen_height / 2};
				screens[2].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};
				
				//disable screen 3 and its attached camera
				screens[3].in_active_use = false;
				screens[3].screen_rect = (Rectangle){game_screen_width / 2,game_screen_height / 2,game_screen_width / 2 ,game_screen_height / 2};
				screens[3].screen_camera = (Rectangle){0,0,game_screen_width / 2,game_screen_height / 2};				
			}
		}
		
	}

private:

	std::uint16_t m_level_bound_left_x;
	std::uint16_t m_level_bound_right_x;
	std::uint16_t m_level_bound_up_y;
	std::uint16_t m_level_bound_down_y;
	
	std::uint8_t m_num_screens_used;
	
};

#endif
