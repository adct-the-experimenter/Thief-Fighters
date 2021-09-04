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
	
	m_gameScreenWidth = gameScreenWidth_float; 
	m_gameScreenHeight = gameScreenHeight_float;
	
	m_camera_manager_ptr->m_num_players = num_players;
	
	level_bound_left_x = 0;
	level_bound_right_x = 6600;
	level_bound_up_y = 0;
	level_bound_down_y = 6240;

	m_camera_manager_ptr->SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
	
	//initialize cameras based on number of players
	switch(num_players)
	{
		case 1:
		{
			//1 camera 
			m_camera_manager_ptr->lead_cameras[0].Init(gameScreenWidth,gameScreenHeight);
			m_camera_manager_ptr->lead_cameras[0].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
			m_camera_manager_ptr->lead_cameras[0].SetCameraLeadPlayerNumber(0);
			
			m_camera_manager_ptr->SetForOneScreen(gameScreenWidth_float,gameScreenHeight_float);
			
			m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[0],0);
			
			break;
		}
		case 2:
		{
			//2 cameras
			for(size_t i = 0; i < 2; i++)
			{				
				m_camera_manager_ptr->lead_cameras[i].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
				m_camera_manager_ptr->lead_cameras[i].SetCameraLeadPlayerNumber(i);
			}
			
			m_camera_manager_ptr->SetForTwoScreens(gameScreenWidth_float,gameScreenHeight_float);
			
			m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[0],0);
			m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[1],1);
			
			break;
		}
		case 3:
		{ 
			//3 cameras in upside down triangle formation
			for(size_t i = 0; i < 3; i++)
			{
				m_camera_manager_ptr->lead_cameras[i].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
				m_camera_manager_ptr->lead_cameras[i].SetCameraLeadPlayerNumber(i);
			}
			
			m_camera_manager_ptr->SetForThreeScreens(gameScreenWidth_float,gameScreenHeight_float);
			
			m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[0],0);
			m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[1],1);
			m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[2],2);
			
			break;
		}
	}
	
	if(num_players >= 4)
	{
		//4 equal size cameras at top and bottom
		for(size_t i = 0; i < 4; i++)
		{
			m_camera_manager_ptr->lead_cameras[i].SetLevelBounds(level_bound_left_x,level_bound_right_x,level_bound_up_y,level_bound_down_y);
			m_camera_manager_ptr->lead_cameras[i].SetCameraLeadPlayerNumber(i);
		}
		
		m_camera_manager_ptr->SetForFourScreens(gameScreenWidth_float,gameScreenHeight_float);
		
		m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[0],0);
		m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[1],1);
		m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[2],2);
		m_camera_manager_ptr->AttachCameraToScreen(&m_camera_manager_ptr->lead_cameras[3],3);
	}
}

static bool CheckCollisionBetweenRectangles(Rectangle& rect_a, Rectangle& rect_b)
{
	
	float a_LeftX = rect_a.x;
	float a_RightX = rect_a.x + rect_a.width;
	float a_TopY = rect_a.y;
	float a_BottomY = rect_a.y + rect_a.height;
	
	float b_LeftX = rect_b.x;
	float b_RightX = rect_b.x + rect_b.width;
	float b_TopY = rect_b.y;
	float b_BottomY = rect_b.y + rect_b.height;
	
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

static bool ShouldAdjacentHorizontalCamerasJoin(CustomCamera* camera_a_ptr, CustomCamera* camera_b_ptr, float& gameScreenWidth,float& gameScreenHeight)
{
	Rectangle* a_rect_ptr = camera_a_ptr->GetCameraRectPointer();
	Rectangle* b_rect_ptr = camera_b_ptr->GetCameraRectPointer();
	
	//if camera rectangles are colliding
	if( CheckCollisionBetweenRectangles(*a_rect_ptr, *b_rect_ptr) )
	{
		float diff_camera_x = abs(b_rect_ptr->x - a_rect_ptr->x);
		
		//if camera B is more than halfway inside camera A, join the cameras
		if(diff_camera_x <= gameScreenWidth)
		{
			return true;
		}
	}
	
	return false;
}

static bool ShouldAdjacentHorizontalCamerasSplit(CustomCamera* camera_a_ptr, CustomCamera* camera_b_ptr, float& gameScreenWidth,float& gameScreenHeight)
{
	Rectangle* a_rect_ptr = camera_a_ptr->GetCameraRectPointer();
	Rectangle* b_rect_ptr = camera_b_ptr->GetCameraRectPointer();
	
	
	//if camera rectangles are colliding
	if( CheckCollisionBetweenRectangles(*a_rect_ptr, *b_rect_ptr) )
	{
		float diff_camera_x = abs(b_rect_ptr->x - a_rect_ptr->x);
		
		//if camera B is more than halfway inside camera A, split the cameras
		//30 is player sprite width
		if( diff_camera_x < 0.5*gameScreenWidth + 30 )
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

static bool ShouldAdjacentVerticalCamerasJoin(CustomCamera* camera_a_ptr, CustomCamera* camera_b_ptr, float& gameScreenWidth,float& gameScreenHeight)
{
	Rectangle* a_rect_ptr = camera_a_ptr->GetCameraRectPointer();
	Rectangle* b_rect_ptr = camera_b_ptr->GetCameraRectPointer();
	
	//if camera rectangles are colliding
	if( CheckCollisionBetweenRectangles(*a_rect_ptr, *b_rect_ptr) )
	{
		float diff_camera_y = abs(b_rect_ptr->y - a_rect_ptr->y);
		
		//if camera B is more than halfway inside camera A, join the cameras
		if(diff_camera_y <= gameScreenHeight )
		{
			return true;
		}
	}
	
	return false;
}

static bool ShouldCollidingCamerasJoin(CustomCamera* camera_a_ptr, CustomCamera* camera_b_ptr)
{
	Rectangle* a_rect_ptr = camera_a_ptr->GetCameraRectPointer();
	Rectangle* b_rect_ptr = camera_b_ptr->GetCameraRectPointer();
	
	float diff_camera_x = abs(b_rect_ptr->x - a_rect_ptr->x);
	float diff_camera_y = abs(b_rect_ptr->y - a_rect_ptr->y);
	
	float dist_square = diff_camera_x*diff_camera_x + diff_camera_y*diff_camera_y;
	
	//if cameras are close i.e. x and y dist components are about half of game screen width or less
	if(dist_square <= 0.5*camera_a_ptr->GetCameraRectPointer()->height*camera_a_ptr->GetCameraRectPointer()->width)
	{
		return true;
	}
	
	return false;
}

static bool ShouldAdjacentVerticalCamerasSplit(CustomCamera* camera_a_ptr, CustomCamera* camera_b_ptr, float& gameScreenWidth,float& gameScreenHeight)
{
	Rectangle* a_rect_ptr = camera_a_ptr->GetCameraRectPointer();
	Rectangle* b_rect_ptr = camera_b_ptr->GetCameraRectPointer();
	
	
	//if camera rectangles are colliding
	if( CheckCollisionBetweenRectangles(*a_rect_ptr, *b_rect_ptr) )
	{
		float diff_camera_y = abs(b_rect_ptr->y - a_rect_ptr->y);
		float diff_camera_x = abs(b_rect_ptr->x - a_rect_ptr->x);
		
		//if camera B is more than halfway inside camera A, split the cameras
		//30 is player sprite width
		if( diff_camera_y < 0.5*a_rect_ptr->height - 60 ||
			diff_camera_x < 0.5*a_rect_ptr->width - 30)
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


#define DEBUG_CAMERA_SYSTEM

void CameraSystem::DetermineCameraConfigTwoPlayers()
{
	CustomCamera* topLeft_camera = nullptr;
	CustomCamera* topRight_camera = nullptr;

	//check which camera is most left

	if(m_camera_manager_ptr->lead_cameras[0].GetCameraRectPointer()->x <= m_camera_manager_ptr->lead_cameras[1].GetCameraRectPointer()->x)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[0];
		topRight_camera = &m_camera_manager_ptr->lead_cameras[1];
	}
	//check which camera is most right
	else
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[1];
		topRight_camera = &m_camera_manager_ptr->lead_cameras[0];
	}

	m_camera_manager_ptr->AttachCameraToScreen(topLeft_camera,0);
	m_camera_manager_ptr->AttachCameraToScreen(topRight_camera,1);
	
	topLeft_camera = m_camera_manager_ptr->screens[0].camera_ptr;
	topRight_camera = m_camera_manager_ptr->screens[1].camera_ptr;
	
	if(topLeft_camera && topRight_camera)
	{
		//determine if screens/cameras zero and one should join or split
		CustomCamera* camera_a_ptr = topLeft_camera;
		CustomCamera* camera_b_ptr = topRight_camera;
		
		//check if cameras need to be joined together
		if( !m_camera_manager_ptr->screens_joined_bitset[1] || !m_camera_manager_ptr->screens_joined_bitset[0])
		{
			if( ShouldAdjacentHorizontalCamerasJoin(camera_a_ptr,camera_b_ptr,m_gameScreenWidth,m_gameScreenHeight) )
			{
				m_camera_manager_ptr->JoinScreenZeroAndScreenOne();
				
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nJoining screen zero and screen one!\n \n";
				#endif
			}
			
		}
		//check if cameras need to be split
		else
		{
			if( ShouldAdjacentHorizontalCamerasSplit(camera_a_ptr,camera_b_ptr,m_gameScreenWidth,m_gameScreenHeight) )
			{
				m_camera_manager_ptr->SplitScreenZeroAndScreenOne();
				
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nSplitting screen zero and screen one!\n \n";
				#endif
			}
		}
	}
	
}

void CameraSystem::DetermineCameraConfigThreePlayers()
{
	CustomCamera* topLeft_camera = nullptr;
	CustomCamera* topRight_camera = nullptr;
	CustomCamera* bottomLeft_camera = nullptr;
	
	Rectangle* camera_zero_ptr = m_camera_manager_ptr->lead_cameras[0].GetCameraRectPointer();
	Rectangle* camera_one_ptr = m_camera_manager_ptr->lead_cameras[1].GetCameraRectPointer();
	Rectangle* camera_two_ptr = m_camera_manager_ptr->lead_cameras[2].GetCameraRectPointer();
	
	bool lead_cameras_assigned[3] = {false,false,false};
	
	//
	//calculate average point of cameras, assuming it works well enough as center
	float camera_zero_center_x, camera_zero_center_y;
	m_camera_manager_ptr->lead_cameras[0].GetLeadPlayerCoordinates(camera_zero_center_x,camera_zero_center_y);
	
	float camera_one_center_x, camera_one_center_y;
	m_camera_manager_ptr->lead_cameras[1].GetLeadPlayerCoordinates(camera_one_center_x,camera_one_center_y);
	
	float camera_two_center_x, camera_two_center_y;
	m_camera_manager_ptr->lead_cameras[2].GetLeadPlayerCoordinates(camera_two_center_x,camera_two_center_y);
	
	float avg_y = (camera_zero_ptr->y + camera_one_ptr->y + camera_two_ptr->y ) / 3;
	
	//determine top left camera
	if(camera_zero_center_x <  camera_one_center_x && camera_zero_center_x < camera_two_center_x
		&& (camera_zero_center_y < avg_y - camera_zero_ptr->height*0.5 || camera_zero_center_y > avg_y + camera_zero_ptr->height*0.5)
		)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[0];
		lead_cameras_assigned[0] = true;
	}
	else if(camera_one_center_x <  camera_zero_center_x && camera_one_center_x < camera_two_center_x
			&& (camera_one_center_y < avg_y - camera_one_ptr->height*0.5 || camera_one_center_y > avg_y + camera_one_ptr->height*0.5)
			)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[1];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_two_center_x < camera_zero_center_x && camera_two_center_x < camera_one_center_x
			&& (camera_two_center_y < avg_y - camera_two_ptr->height*0.5 || camera_two_center_y > avg_y + camera_two_ptr->height*0.5) 
			)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[2];
		lead_cameras_assigned[2] = true;
	}
	
	//determine top right camera
	if(camera_zero_center_x >  camera_one_center_x && camera_zero_center_x > camera_two_center_x
		&& (camera_zero_center_y < avg_y - camera_zero_ptr->height*0.5 || camera_zero_center_y > avg_y + camera_zero_ptr->height*0.5)
		)
	{
		topRight_camera = &m_camera_manager_ptr->lead_cameras[0];
		lead_cameras_assigned[0] = true;
	}
	else if(camera_one_center_x >  camera_zero_center_x && camera_one_center_x > camera_two_center_x
			&& (camera_one_center_y < avg_y - camera_one_ptr->height*0.5 || camera_one_center_y > avg_y + camera_one_ptr->height*0.5)
			)
	{
		topRight_camera = &m_camera_manager_ptr->lead_cameras[1];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_two_center_x > camera_zero_center_x && camera_two_center_x > camera_one_center_x
			&& (camera_two_center_y < avg_y - camera_two_ptr->height*0.5 || camera_two_center_y > avg_y + camera_two_ptr->height*0.5) 
			)
	{
		topRight_camera = &m_camera_manager_ptr->lead_cameras[2];
		lead_cameras_assigned[2] = true;
	}
	
	//determine bottom left camera
	if(camera_zero_center_x <  camera_one_center_x && camera_zero_center_x < camera_two_center_x
		&& (camera_zero_center_y < avg_y - camera_zero_ptr->height*0.5 || camera_zero_center_y > avg_y + camera_zero_ptr->height*0.5)
		&& !lead_cameras_assigned[0]
		)
	{
		bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[0];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_one_center_x <  camera_zero_center_x && camera_one_center_x < camera_two_center_x
			&& (camera_one_center_y < avg_y - camera_one_ptr->height*0.5 || camera_one_center_y > avg_y + camera_one_ptr->height*0.5)
			&& !lead_cameras_assigned[1]
			)
	{
		bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[1];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_two_center_x < camera_zero_center_x && camera_two_center_x < camera_one_center_x
			&& (camera_two_center_y < avg_y - camera_two_ptr->height*0.5 || camera_two_center_y > avg_y + camera_two_ptr->height*0.5)
			&& !lead_cameras_assigned[2] 
			)
	{
		bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[2];
		lead_cameras_assigned[2] = true;
	}
	
	//if the cameras have not been assigned
	
	if(!topLeft_camera)
	{
		if(!lead_cameras_assigned[0]){topLeft_camera = &m_camera_manager_ptr->lead_cameras[0];}
		else if(!lead_cameras_assigned[1]){topLeft_camera = &m_camera_manager_ptr->lead_cameras[1];}
		else if(!lead_cameras_assigned[2]){topLeft_camera = &m_camera_manager_ptr->lead_cameras[2];}
	}
	
	if(!topRight_camera)
	{
		if(!lead_cameras_assigned[0]){topRight_camera = &m_camera_manager_ptr->lead_cameras[0];}
		else if(!lead_cameras_assigned[1]){topRight_camera = &m_camera_manager_ptr->lead_cameras[1];}
		else if(!lead_cameras_assigned[2]){topRight_camera = &m_camera_manager_ptr->lead_cameras[2];}
	}
	
	if(!bottomLeft_camera)
	{
		if(!lead_cameras_assigned[0]){bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[0];}
		else if(!lead_cameras_assigned[1]){bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[1];}
		else if(!lead_cameras_assigned[2]){bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[2];}
	}
	
	m_camera_manager_ptr->AttachCameraToScreen(topLeft_camera,0);
	m_camera_manager_ptr->AttachCameraToScreen(topRight_camera,1);
	m_camera_manager_ptr->AttachCameraToScreen(bottomLeft_camera,2);
		
	//check collisions between camera rectangles
	
	//join screens of colliding camera rectangles
	//split screens of non-colliding camera rectangles
	
	//if top left, top right, bottom left cameras are colliding together
	if(topLeft_camera && topRight_camera && bottomLeft_camera)
	{
		
		bool collide_01 = CheckCollisionBetweenRectangles(*topLeft_camera->GetCameraRectPointer(), *topRight_camera->GetCameraRectPointer());
		bool collide_02 = CheckCollisionBetweenRectangles(*topLeft_camera->GetCameraRectPointer(),*bottomLeft_camera->GetCameraRectPointer());
		bool collide_12 = CheckCollisionBetweenRectangles(*topRight_camera->GetCameraRectPointer(),*bottomLeft_camera->GetCameraRectPointer());
		
		#ifdef DEBUG_CAMERA_SYSTEM
		std::cout << "\ntopLeft_camera: " << topLeft_camera->GetCameraRectPointer()->x << " , " << topLeft_camera->GetCameraRectPointer()->y 
										<< " , " << topLeft_camera->GetCameraRectPointer()->width << " , " << topLeft_camera->GetCameraRectPointer()->height;
		
		std::cout << "\ntopRightCamera: " << topRight_camera->GetCameraRectPointer()->x << " , " << topRight_camera->GetCameraRectPointer()->y 
										<< " , " << topRight_camera->GetCameraRectPointer()->width << " , " << topRight_camera->GetCameraRectPointer()->height;
										
		std::cout << "\nbottomLeft_camera: " << bottomLeft_camera->GetCameraRectPointer()->x << " , " << bottomLeft_camera->GetCameraRectPointer()->y 
										<< " , " << bottomLeft_camera->GetCameraRectPointer()->width << " , " << bottomLeft_camera->GetCameraRectPointer()->height;
		
		std::cout << "\ncollide 01: " << collide_01 << "\ncollide 02: " << collide_02 << "\ncollide 12: " << collide_12 << std::endl;
		#endif
		
		if( (collide_01 && collide_02) || (collide_02 && collide_12) || (collide_01 && collide_12))
		{
			#ifdef DEBUG_CAMERA_SYSTEM
			std::cout << "\nJoining all 3 screens\n";
			#endif
			//join all 3 screens
			m_camera_manager_ptr->screens_joined_bitset.set(0);
			m_camera_manager_ptr->screens_joined_bitset.set(1);
			m_camera_manager_ptr->screens_joined_bitset.set(2);
			m_camera_manager_ptr->ApplyNewScreenState();
		}
		else if( collide_01 && !collide_02 && !collide_12)
		{
			if(ShouldCollidingCamerasJoin(topLeft_camera,topRight_camera) )
			{
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nJoining screens 0 and 1, split screen 2\n";
				#endif
				//join screen 0 and screen 1, split screen 2
				m_camera_manager_ptr->screens_joined_bitset.set(0);
				m_camera_manager_ptr->screens_joined_bitset.set(1);
				m_camera_manager_ptr->screens_joined_bitset.reset(2);
				m_camera_manager_ptr->ApplyNewScreenState();
			}
			else
			{
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nSplitting screens 0 and 1, split screen 2\n";
				#endif
				//split screen 0 and screen 1, split screen 2
				m_camera_manager_ptr->screens_joined_bitset.reset(0);
				m_camera_manager_ptr->screens_joined_bitset.reset(1);
				m_camera_manager_ptr->screens_joined_bitset.reset(2);
				m_camera_manager_ptr->ApplyNewScreenState();
			}
			
		}
		else if(!collide_01 && collide_02 && !collide_12)
		{
			if(ShouldCollidingCamerasJoin(topLeft_camera,bottomLeft_camera) )
			{
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nJoining screens 0 and 2, split screen 1\n";
				#endif
				//join screen 0 and screen 2, split screen 1
				m_camera_manager_ptr->screens_joined_bitset.set(0);
				m_camera_manager_ptr->screens_joined_bitset.set(2);
				m_camera_manager_ptr->screens_joined_bitset.reset(1);
				m_camera_manager_ptr->ApplyNewScreenState();
			}
			else
			{
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nSplitting screens 0 and 2, split screen 1\n";
				#endif
				//split screen 0 and screen 2, split screen 1
				m_camera_manager_ptr->screens_joined_bitset.reset(0);
				m_camera_manager_ptr->screens_joined_bitset.reset(2);
				m_camera_manager_ptr->screens_joined_bitset.reset(1);
				m_camera_manager_ptr->ApplyNewScreenState();
			}
		}
		else if(!collide_01 && !collide_02 && collide_12)
		{
			if(ShouldCollidingCamerasJoin(topRight_camera,bottomLeft_camera) )
			{
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nJoining screens 1 and 2, split screen 0\n";
				#endif
				//join screen 0 and screen 2, split screen 1
				m_camera_manager_ptr->screens_joined_bitset.set(1);
				m_camera_manager_ptr->screens_joined_bitset.set(2);
				m_camera_manager_ptr->screens_joined_bitset.reset(0);
				m_camera_manager_ptr->ApplyNewScreenState();
			}
			else
			{
				#ifdef DEBUG_CAMERA_SYSTEM
				std::cout << "\nSplitting screens 1 and 2, split screen 0\n";
				#endif
				//split screen 0 and screen 2, split screen 1
				m_camera_manager_ptr->screens_joined_bitset.reset(1);
				m_camera_manager_ptr->screens_joined_bitset.reset(2);
				m_camera_manager_ptr->screens_joined_bitset.reset(0);
				m_camera_manager_ptr->ApplyNewScreenState();
			}
			
		}
		//else if none of the cameras are colliding
		else if(!collide_01 && !collide_02 && !collide_12)
		{
			#ifdef DEBUG_CAMERA_SYSTEM
			std::cout << "\nSplitting all 3 screens.\n";
			#endif
			//split all 3 screens
			m_camera_manager_ptr->screens_joined_bitset.reset(0);
			m_camera_manager_ptr->screens_joined_bitset.reset(1);
			m_camera_manager_ptr->screens_joined_bitset.reset(2);
			m_camera_manager_ptr->ApplyNewScreenState();
		}
	}
	
	
	
}

void CameraSystem::DetermineCameraConfigFourPlusPlayers()
{
	CustomCamera* topLeft_camera = nullptr;
	CustomCamera* topRight_camera = nullptr;
	CustomCamera* bottomLeft_camera = nullptr;
	CustomCamera* bottomRight_camera = nullptr;
	
	Rectangle* camera_zero_ptr = m_camera_manager_ptr->lead_cameras[0].GetCameraRectPointer();
	Rectangle* camera_one_ptr = m_camera_manager_ptr->lead_cameras[1].GetCameraRectPointer();
	Rectangle* camera_two_ptr = m_camera_manager_ptr->lead_cameras[2].GetCameraRectPointer();
	Rectangle* camera_three_ptr = m_camera_manager_ptr->lead_cameras[3].GetCameraRectPointer();
	
	bool lead_cameras_assigned[4] = {false,false,false,false};
	
	//
	//calculate average point of cameras, assuming it works well enough as center
	float camera_zero_center_x, camera_zero_center_y;
	m_camera_manager_ptr->lead_cameras[0].GetLeadPlayerCoordinates(camera_zero_center_x,camera_zero_center_y);
	
	float camera_one_center_x, camera_one_center_y;
	m_camera_manager_ptr->lead_cameras[1].GetLeadPlayerCoordinates(camera_one_center_x,camera_one_center_y);
	
	float camera_two_center_x, camera_two_center_y;
	m_camera_manager_ptr->lead_cameras[2].GetLeadPlayerCoordinates(camera_two_center_x,camera_two_center_y);
	
	float camera_three_center_x, camera_three_center_y;
	m_camera_manager_ptr->lead_cameras[3].GetLeadPlayerCoordinates(camera_three_center_x,camera_three_center_y);
	
	float avg_y = (camera_zero_ptr->y + camera_one_ptr->y + camera_two_ptr->y + camera_three_ptr->y) / 4;
	
	//determine top left camera
	if(camera_zero_center_x <  camera_one_center_x && camera_zero_center_x < camera_two_center_x && camera_zero_center_x < camera_three_center_x
		&& (camera_zero_center_y < avg_y - camera_zero_ptr->height*0.5 || camera_zero_center_y > avg_y + camera_zero_ptr->height*0.5)
		)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[0];
		lead_cameras_assigned[0] = true;
	}
	else if(camera_one_center_x <  camera_zero_center_x && camera_one_center_x < camera_two_center_x && camera_one_center_x < camera_three_center_x
			&& (camera_one_center_y < avg_y - camera_one_ptr->height*0.5 || camera_one_center_y > avg_y + camera_one_ptr->height*0.5)
			)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[1];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_two_center_x < camera_zero_center_x && camera_two_center_x < camera_one_center_x && camera_two_center_x < camera_three_center_x
			&& (camera_two_center_y < avg_y - camera_two_ptr->height*0.5 || camera_two_center_y > avg_y + camera_two_ptr->height*0.5) 
			)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[2];
		lead_cameras_assigned[2] = true;
	}
	else if(camera_three_center_x < camera_zero_center_x && camera_three_center_x < camera_one_center_x && camera_three_center_x < camera_two_center_x
			&& (camera_three_center_y < avg_y - camera_three_ptr->height*0.5 || camera_three_center_y > avg_y + camera_three_ptr->height*0.5) 
			)
	{
		topLeft_camera = &m_camera_manager_ptr->lead_cameras[3];
		lead_cameras_assigned[3] = true;
	}
	
	//determine top right camera
	if(camera_zero_center_x >  camera_one_center_x && camera_zero_center_x > camera_two_center_x && camera_zero_center_x > camera_three_center_x
		&& (camera_zero_center_y < avg_y - camera_zero_ptr->height*0.5 || camera_zero_center_y > avg_y + camera_zero_ptr->height*0.5)
		)
	{
		topRight_camera = &m_camera_manager_ptr->lead_cameras[0];
		lead_cameras_assigned[0] = true;
	}
	else if(camera_one_center_x >  camera_zero_center_x && camera_one_center_x > camera_two_center_x && camera_one_center_x > camera_three_center_x
			&& (camera_one_center_y < avg_y - camera_one_ptr->height*0.5 || camera_one_center_y > avg_y + camera_one_ptr->height*0.5)
			)
	{
		topRight_camera = &m_camera_manager_ptr->lead_cameras[1];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_two_center_x > camera_zero_center_x && camera_two_center_x > camera_one_center_x && camera_two_center_x > camera_three_center_x
			&& (camera_two_center_y < avg_y - camera_two_ptr->height*0.5 || camera_two_center_y > avg_y + camera_two_ptr->height*0.5) 
			)
	{
		topRight_camera = &m_camera_manager_ptr->lead_cameras[2];
		lead_cameras_assigned[2] = true;
	}
	else if(camera_three_center_x > camera_zero_center_x && camera_three_center_x > camera_one_center_x && camera_three_center_x > camera_two_center_x
			&& (camera_three_center_y < avg_y - camera_three_ptr->height*0.5 || camera_three_center_y > avg_y + camera_three_ptr->height*0.5) 
			)
	{
		topRight_camera = &m_camera_manager_ptr->lead_cameras[3];
		lead_cameras_assigned[3] = true;
	}
	
	//determine bottom left camera
	if(camera_zero_center_x <  camera_one_center_x && camera_zero_center_x < camera_two_center_x && camera_zero_center_x < camera_three_center_x
		&& (camera_zero_center_y < avg_y - camera_zero_ptr->height*0.5 || camera_zero_center_y > avg_y + camera_zero_ptr->height*0.5)
		&& !lead_cameras_assigned[0]
		)
	{
		bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[0];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_one_center_x <  camera_zero_center_x && camera_one_center_x < camera_two_center_x && camera_one_center_x < camera_three_center_x
			&& (camera_one_center_y < avg_y - camera_one_ptr->height*0.5 || camera_one_center_y > avg_y + camera_one_ptr->height*0.5)
			&& !lead_cameras_assigned[1]
			)
	{
		bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[1];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_two_center_x < camera_zero_center_x && camera_two_center_x < camera_one_center_x && camera_two_center_x < camera_three_center_x
			&& (camera_two_center_y < avg_y - camera_two_ptr->height*0.5 || camera_two_center_y > avg_y + camera_two_ptr->height*0.5)
			&& !lead_cameras_assigned[2] 
			)
	{
		bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[2];
		lead_cameras_assigned[2] = true;
	}
	else if(camera_three_center_x < camera_zero_center_x && camera_three_center_x < camera_one_center_x && camera_three_center_x < camera_two_center_x
			&& (camera_three_center_y < avg_y - camera_three_ptr->height*0.5 || camera_three_center_y > avg_y + camera_three_ptr->height*0.5)
			&& !lead_cameras_assigned[3] 
			)
	{
		bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[3];
		lead_cameras_assigned[3] = true;
	}
	
	//determine bottom right camera
	if(camera_zero_center_x >  camera_one_center_x && camera_zero_center_x > camera_two_center_x && camera_zero_center_x > camera_three_center_x
		&& (camera_zero_center_y < avg_y - camera_zero_ptr->height*0.5 || camera_zero_center_y > avg_y + camera_zero_ptr->height*0.5)
		&& !lead_cameras_assigned[0]
		)
	{
		bottomRight_camera = &m_camera_manager_ptr->lead_cameras[0];
		lead_cameras_assigned[0] = true;
	}
	else if(camera_one_center_x >  camera_zero_center_x && camera_one_center_x > camera_two_center_x && camera_one_center_x > camera_three_center_x
			&& (camera_one_center_y < avg_y - camera_one_ptr->height*0.5 || camera_one_center_y > avg_y + camera_one_ptr->height*0.5)
			&& !lead_cameras_assigned[1]
			)
	{
		bottomRight_camera = &m_camera_manager_ptr->lead_cameras[1];
		lead_cameras_assigned[1] = true;
	}
	else if(camera_two_center_x > camera_zero_center_x && camera_two_center_x > camera_one_center_x && camera_two_center_x > camera_three_center_x
			&& (camera_two_center_y < avg_y - camera_two_ptr->height*0.5 || camera_two_center_y > avg_y + camera_two_ptr->height*0.5) 
			&& !lead_cameras_assigned[2]
			)
	{
		bottomRight_camera = &m_camera_manager_ptr->lead_cameras[2];
		lead_cameras_assigned[2] = true;
	}
	else if(camera_three_center_x > camera_zero_center_x && camera_three_center_x > camera_one_center_x && camera_three_center_x > camera_two_center_x
			&& (camera_three_center_y < avg_y - camera_three_ptr->height*0.5 || camera_three_center_y > avg_y + camera_three_ptr->height*0.5) 
			&& !lead_cameras_assigned[3]
			)
	{
		bottomRight_camera = &m_camera_manager_ptr->lead_cameras[3];
		lead_cameras_assigned[3] = true;
	}
	
	//if the cameras have not been assigned
	
	if(!topLeft_camera)
	{
		if(!lead_cameras_assigned[0]){topLeft_camera = &m_camera_manager_ptr->lead_cameras[0];}
		else if(!lead_cameras_assigned[1]){topLeft_camera = &m_camera_manager_ptr->lead_cameras[1];}
		else if(!lead_cameras_assigned[2]){topLeft_camera = &m_camera_manager_ptr->lead_cameras[2];}
		else if(!lead_cameras_assigned[3]){topLeft_camera = &m_camera_manager_ptr->lead_cameras[3];}
	}
	
	if(!topRight_camera)
	{
		if(!lead_cameras_assigned[0]){topRight_camera = &m_camera_manager_ptr->lead_cameras[0];}
		else if(!lead_cameras_assigned[1]){topRight_camera = &m_camera_manager_ptr->lead_cameras[1];}
		else if(!lead_cameras_assigned[2]){topRight_camera = &m_camera_manager_ptr->lead_cameras[2];}
		else if(!lead_cameras_assigned[3]){topRight_camera = &m_camera_manager_ptr->lead_cameras[3];}
	}
	
	if(!bottomLeft_camera)
	{
		if(!lead_cameras_assigned[0]){bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[0];}
		else if(!lead_cameras_assigned[1]){bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[1];}
		else if(!lead_cameras_assigned[2]){bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[2];}
		else if(!lead_cameras_assigned[3]){bottomLeft_camera = &m_camera_manager_ptr->lead_cameras[3];}
	}
	
	if(!bottomRight_camera)
	{
		if(!lead_cameras_assigned[0]){bottomRight_camera = &m_camera_manager_ptr->lead_cameras[0];}
		else if(!lead_cameras_assigned[1]){bottomRight_camera = &m_camera_manager_ptr->lead_cameras[1];}
		else if(!lead_cameras_assigned[2]){bottomRight_camera = &m_camera_manager_ptr->lead_cameras[2];}
		else if(!lead_cameras_assigned[3]){bottomRight_camera = &m_camera_manager_ptr->lead_cameras[3];}
	}
	
	m_camera_manager_ptr->AttachCameraToScreen(topLeft_camera,0);
	m_camera_manager_ptr->AttachCameraToScreen(topRight_camera,1);
	m_camera_manager_ptr->AttachCameraToScreen(bottomLeft_camera,2);
	m_camera_manager_ptr->AttachCameraToScreen(bottomRight_camera,3);
		
	//check collisions between camera rectangles
	
	//join screens of colliding camera rectangles
	//split screens of non-colliding camera rectangles
	
	//if top left, top right, bottom left cameras are colliding together
	if(topLeft_camera && topRight_camera && bottomLeft_camera && bottomRight_camera)
	{
		
		bool collide_01 = CheckCollisionBetweenRectangles(*topLeft_camera->GetCameraRectPointer(), *topRight_camera->GetCameraRectPointer());
		bool collide_02 = CheckCollisionBetweenRectangles(*topLeft_camera->GetCameraRectPointer(),*bottomLeft_camera->GetCameraRectPointer());
		bool collide_03 = CheckCollisionBetweenRectangles(*topLeft_camera->GetCameraRectPointer(),*bottomRight_camera->GetCameraRectPointer());
		
		bool collide_12 = CheckCollisionBetweenRectangles(*topRight_camera->GetCameraRectPointer(),*bottomLeft_camera->GetCameraRectPointer());
		bool collide_13 = CheckCollisionBetweenRectangles(*topRight_camera->GetCameraRectPointer(),*bottomRight_camera->GetCameraRectPointer());
		bool collide_23 = CheckCollisionBetweenRectangles(*bottomLeft_camera->GetCameraRectPointer(),*bottomRight_camera->GetCameraRectPointer());
		
		#ifdef DEBUG_CAMERA_SYSTEM
		std::cout << "\ntopLeft_camera: " << topLeft_camera->GetCameraRectPointer()->x << " , " << topLeft_camera->GetCameraRectPointer()->y 
										<< " , " << topLeft_camera->GetCameraRectPointer()->width << " , " << topLeft_camera->GetCameraRectPointer()->height;
		
		std::cout << "\ntopRightCamera: " << topRight_camera->GetCameraRectPointer()->x << " , " << topRight_camera->GetCameraRectPointer()->y 
										<< " , " << topRight_camera->GetCameraRectPointer()->width << " , " << topRight_camera->GetCameraRectPointer()->height;
										
		std::cout << "\nbottomLeft_camera: " << bottomLeft_camera->GetCameraRectPointer()->x << " , " << bottomLeft_camera->GetCameraRectPointer()->y 
										<< " , " << bottomLeft_camera->GetCameraRectPointer()->width << " , " << bottomLeft_camera->GetCameraRectPointer()->height;
		
		std::cout << "\nbottomRight_camera: " << bottomRight_camera->GetCameraRectPointer()->x << " , " << bottomRight_camera->GetCameraRectPointer()->y 
										<< " , " << bottomRight_camera->GetCameraRectPointer()->width << " , " << bottomRight_camera->GetCameraRectPointer()->height;
		
		std::cout << "\ncollide 01: " << collide_01 
				<< "\ncollide 02: " << collide_02 
				<< "\ncollide 03: " << collide_03 
				<< "\ncollide 12: " << collide_12
				<< "\ncollide 13: " << collide_13
				 << std::endl;
		#endif
		
		if( (collide_01 && collide_02 && collide_03) || (collide_01 && collide_12 && collide_13) 
			|| (collide_02 && collide_12 && collide_23) || (collide_03 && collide_13 && collide_23) )
		{
			#ifdef DEBUG_CAMERA_SYSTEM
			std::cout << "\nJoining all 4 screens\n";
			#endif
			//join all 3 screens
			m_camera_manager_ptr->screens_joined_bitset.set(0);
			m_camera_manager_ptr->screens_joined_bitset.set(1);
			m_camera_manager_ptr->screens_joined_bitset.set(2);
			m_camera_manager_ptr->screens_joined_bitset.set(3);
			m_camera_manager_ptr->ApplyNewScreenState();
		}
		else if( collide_01 && !collide_02 && !collide_12)
		{
			#ifdef DEBUG_CAMERA_SYSTEM
			std::cout << "\nJoining screens 0 and 1, split screen 2\n";
			#endif
			//join screen 0 and screen 1, split screen 2
			m_camera_manager_ptr->screens_joined_bitset.set(0);
			m_camera_manager_ptr->screens_joined_bitset.set(1);
			m_camera_manager_ptr->screens_joined_bitset.reset(2);
			m_camera_manager_ptr->ApplyNewScreenState();
		}
		else if(!collide_01 && collide_02 && !collide_12)
		{
			#ifdef DEBUG_CAMERA_SYSTEM
			std::cout << "\nJoining screens 0 and 2, split screen 1\n";
			#endif
			//join screen 0 and screen 2, split screen 1
			m_camera_manager_ptr->screens_joined_bitset.set(0);
			m_camera_manager_ptr->screens_joined_bitset.set(2);
			m_camera_manager_ptr->screens_joined_bitset.reset(1);
			m_camera_manager_ptr->ApplyNewScreenState();
		}
		//else if none of the cameras are colliding
		else if(!collide_01 && !collide_02 && !collide_03 
				&& !collide_12 && !collide_13 && !collide_23)
		{
			#ifdef DEBUG_CAMERA_SYSTEM
			std::cout << "\nSplitting all 4 screens.\n";
			#endif
			//split all 3 screens
			m_camera_manager_ptr->screens_joined_bitset.reset(0);
			m_camera_manager_ptr->screens_joined_bitset.reset(1);
			m_camera_manager_ptr->screens_joined_bitset.reset(2);
			m_camera_manager_ptr->screens_joined_bitset.reset(3);
			m_camera_manager_ptr->ApplyNewScreenState();
		}
	}
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
			
			//pass lead player coordinates to camera
			m_camera_manager_ptr->lead_cameras[ player.camera_num_leader ].SetLeadPlayerCoordinates(transform.position.x,transform.position.y);
			
			//center camera to player
			camera_rect->x = transform.position.x - (camera_rect->width / 2);
			camera_rect->y = transform.position.y - (camera_rect->height / 2);
			
			//level bounds
			if(camera_rect->x < level_bound_left_x){camera_rect->x = level_bound_left_x;}
			if(camera_rect->x > level_bound_right_x){camera_rect->x = level_bound_right_x;}
			
			if(camera_rect->y < level_bound_up_y){camera_rect->y = level_bound_up_y;}
			if(camera_rect->y > level_bound_down_y){camera_rect->y = level_bound_down_y;}
			
			#ifdef DEBUG_CAMERA_SYSTEM
			//std::cout << "\nPlayer camera " << int(player.camera_num_leader) << " is updated!\n "
			//<< "camera pos: " << camera_rect->x << " , " << camera_rect->y << " ," 
			//<< camera_rect->width <<  " , " << camera_rect->height << std::endl;
			#endif
		}
	}
	
	//attach camera to screen based on camera position
	//split and join cameras based on camera position
	
	
	if(m_num_players == 2)
	{
		DetermineCameraConfigTwoPlayers();
	}
	else if(m_num_players == 3)
	{
		DetermineCameraConfigThreePlayers();
		
	}
	else if(m_num_players >= 4)
	{
		DetermineCameraConfigFourPlusPlayers();
	}
		
	//update screen camera location based on camera lead coordinates and screen camera dimensions.
	m_camera_manager_ptr->UpdateScreenCameraLocation();
}



void CameraSystem::update_timer(float& dt){zoom_time_counter += dt;}
