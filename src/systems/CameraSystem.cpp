#include "CameraSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>
#include <cmath>

extern Coordinator gCoordinator;


void CameraSystem::Init(CustomCamera* camera, std::uint8_t num_players)
{
	m_camera_ptr = camera;
	m_num_players = num_players;
	
	zoom_time_counter = 0;
	
	level_bound_left_x = 0;
	level_bound_right_x = 6600;
	level_bound_up_y = 0;
	level_bound_down_y = 6240;

}

void CameraSystem::Init_MetroidVaniaMode(CameraManager* camera_m_ptr, std::uint8_t num_players, 
										std::uint32_t gameScreenWidth, std::uint32_t gameScreenHeight)
{
	m_camera_manager_ptr = camera_m_ptr;
	m_num_players = num_players;
	
	float gameScreenWidth_float = static_cast <float> (gameScreenWidth);
	float gameScreenHeight_float = static_cast <float> (gameScreenHeight); 
	
	m_camera_manager_ptr->game_screen_width = gameScreenWidth_float;
	m_camera_manager_ptr->game_screen_height = gameScreenHeight_float;
	
	//initialize cameras based on number of players
	switch(num_players)
	{
		case 1:
		{
			//1 camera 
			m_camera_manager_ptr->lead_cameras[0].Init(gameScreenWidth,gameScreenHeight);
			m_camera_manager_ptr->lead_cameras[0].SetCameraActiveStatus(true);
			m_camera_manager_ptr->lead_cameras[0].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
			m_camera_manager_ptr->lead_cameras[0].SetCameraLeadPlayerNumber(0);
			
			m_camera_manager_ptr->SetForOneScreen(gameScreenWidth_float,gameScreenHeight_float);
			break;
		}
		case 2:
		{
			//2 cameras
			for(size_t i = 0; i < 2; i++)
			{				
				m_camera_manager_ptr->lead_cameras[i].SetCameraActiveStatus(true);
				m_camera_manager_ptr->lead_cameras[i].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
				m_camera_manager_ptr->lead_cameras[i].SetCameraLeadPlayerNumber(i);
			}
			
			m_camera_manager_ptr->SetForTwoScreens(gameScreenWidth_float,gameScreenHeight_float);
			
			break;
		}
		case 3:
		{ 
			//3 cameras in upside down triangle formation
			for(size_t i = 0; i < 3; i++)
			{
				m_camera_manager_ptr->lead_cameras[i].Init(gameScreenWidth / 2,gameScreenHeight / 2);
				m_camera_manager_ptr->lead_cameras[i].SetCameraActiveStatus(true);
				m_camera_manager_ptr->lead_cameras[i].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
				m_camera_manager_ptr->lead_cameras[i].SetCameraLeadPlayerNumber(i);
			}
			
			m_camera_manager_ptr->SetForThreeScreens(gameScreenWidth_float,gameScreenHeight_float);
			
			break;
		}
	}
	
	if(num_players >= 4)
	{
		//4 equal size cameras at top and bottom
		for(size_t i = 0; i < 4; i++)
		{
			m_camera_manager_ptr->lead_cameras[i].Init(gameScreenWidth / 2,gameScreenHeight / 2);
			m_camera_manager_ptr->lead_cameras[i].SetCameraActiveStatus(true);
			m_camera_manager_ptr->lead_cameras[i].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
			m_camera_manager_ptr->lead_cameras[i].SetCameraLeadPlayerNumber(i);
		}
		
		m_camera_manager_ptr->SetForFourScreens(gameScreenWidth_float,gameScreenHeight_float);
	}
}

static bool CheckCollisionBetweenRectangles(Rectangle& rect_a, Rectangle& rect_b)
{
	
	float a_LeftX = rect_a.x;
	float a_RightX = rect_a.x + rect_a.width;
	float a_TopY = rect_a.y;
	float a_BottomY = rect_a.y + rect_a.height;
	
	std::uint32_t b_LeftX = rect_b.x;
	std::uint32_t b_RightX = rect_b.x + rect_b.width;
	std::uint32_t b_TopY = rect_b.y;
	std::uint32_t b_BottomY = rect_b.y + rect_b.height;
	
	//for collision to be true, all conditions must be true. AABB square collsion detection, all
	//The left edge x-position of [A] must be less than the right edge x-position of [B].
    //The right edge x-position of [A] must be greater than the left edge x-position of [B].
    //The top edge y-position of [A] must be less than the bottom edge y-position of [B].
    //The bottom edge y-position of [A] must be greater than the top edge y-position of [B].
    
    if(a_BottomY <= b_TopY)
	{
		return false;
	}
	
	if(a_TopY >= b_BottomY)
	{
		return false;
	}
    
    if(a_RightX <= b_LeftX)
	{
		return false;
	}
	
	if(a_LeftX >= b_RightX)
	{
		return false;
	}
	
	return true;
}

static bool ShouldAdjacentHorizontalCamerasJoin(CustomCamera* camera_a_ptr, CustomCamera* camera_b_ptr)
{
	Rectangle* a_rect_ptr = camera_a_ptr->GetCameraRectPointer();
	Rectangle* b_rect_ptr = camera_b_ptr->GetCameraRectPointer();
	
	//if camera rectangles are colliding
	if( CheckCollisionBetweenRectangles(*a_rect_ptr, *b_rect_ptr) )
	{
		float diff_camera_x = abs(b_rect_ptr->x - a_rect_ptr->x);
		
		//if camera B is more than halfway inside camera A, join the cameras
		if(diff_camera_x < 2*a_rect_ptr->width)
		{
			return true;
		}
	}
	
	return false;
}

static bool ShouldAdjacentHorizontalCamerasSplit(CustomCamera* camera_a_ptr, CustomCamera* camera_b_ptr)
{
	Rectangle* a_rect_ptr = camera_a_ptr->GetCameraRectPointer();
	Rectangle* b_rect_ptr = camera_b_ptr->GetCameraRectPointer();
	
	
	//if camera rectangles are colliding
	if( CheckCollisionBetweenRectangles(*a_rect_ptr, *b_rect_ptr) )
	{
		float diff_camera_x = abs(b_rect_ptr->x - a_rect_ptr->x);
		
		//if camera B is more than halfway inside camera A, split the cameras
		if( diff_camera_x < 0.5*a_rect_ptr->width)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	return true;
}


void CameraSystem::Update_MetroidVaniaMode()
{
	//for 4 or less players
	//number of rectangle clips of world = number of players
	//follow camera leads
	
	
	//for 5 or more players, handle only 4 rectangle clips of world
	//follow camera leads
	//implement bound check on player to prevent non-camera lead player from moving out of
	//camera bounds
	
	//Change camera configurations depending on how close camera leads are to each other
	//if one camera lead is 3/4 inside another camera lead's camera join together.
	//else if one camera lead is 3/4 outside another camera lead's camera split.
	
		
	//for metroidvania game mode
	for (auto const& entity : mEntities)
	{
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
	
		if(player.camera_lead)
		{
			Rectangle* camera_rect = m_camera_manager_ptr->lead_cameras[ player.camera_num_leader ].GetCameraRectPointer();
			
			//center camera to player
			camera_rect->x = transform.position.x - (camera_rect->width / 2);
			camera_rect->y = transform.position.y - (camera_rect->height / 2);
			
			//level bounds
			if(camera_rect->x < level_bound_left_x){camera_rect->x = level_bound_left_x;}
			if(camera_rect->x > level_bound_right_x){camera_rect->x = level_bound_right_x;}
			
			if(camera_rect->y < level_bound_up_y){camera_rect->y = level_bound_up_y;}
			if(camera_rect->y > level_bound_down_y){camera_rect->y = level_bound_down_y;}
			
			//std::cout << "\nPlayer camera " << int(player.camera_num_leader) << " is updated!\n "
			//<< "camera pos: " << camera_rect->x << " , " << camera_rect->y << " ," 
			//<< camera_rect->width <<  " , " << camera_rect->height << std::endl;
			
		}
	}
	
	//check if cameras and screens should be joined or separated
	CustomCamera* camera_a_ptr = nullptr;
	CustomCamera* camera_b_ptr = nullptr;
	
	camera_a_ptr = &m_camera_manager_ptr->lead_cameras[0];
	camera_b_ptr = &m_camera_manager_ptr->lead_cameras[1];
	
	//check if cameras need to be joined together
	if( camera_a_ptr->GetCameraActiveStatus() && camera_b_ptr->GetCameraActiveStatus())
	{
		if( ShouldAdjacentHorizontalCamerasJoin(camera_a_ptr,camera_b_ptr) )
		{
			m_camera_manager_ptr->JoinScreenZeroAndScreenOne();
		}
	}
	else
	{
		if( ShouldAdjacentHorizontalCamerasSplit(camera_a_ptr,camera_b_ptr) )
		{
			m_camera_manager_ptr->SplitScreenZeroAndScreenOne();
		}
	}
	
}



void CameraSystem::update_timer(float& dt){zoom_time_counter += dt;}
