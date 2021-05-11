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

}

void CameraSystem::Update(float& scale)
{
	
	std::uint8_t current_player = 0;
	
	
	Rectangle* camera_rect_ptr = m_camera_ptr->GetCameraRectPointer();
	
	
	//get level bounds
	std::uint16_t& boundDown = m_camera_ptr->GetDownLevelBound();
	std::uint16_t& boundUp = m_camera_ptr->GetUpLevelBound();
	std::uint16_t& boundLeft = m_camera_ptr->GetLeftLevelBound();
	std::uint16_t& boundRight = m_camera_ptr->GetRightLevelBound();
	
	float farthest_left = 0;
	float farthest_right = 0;
	
	float far_left_player_width = 30;
	float far_right_player_width = 30;
	
	float player_height = 60;
	
	
	//for each player
	for (auto const& entity : mEntities)
	{
		
		auto& transform = gCoordinator.GetComponent <Transform2D> (entity);
		auto& collisionBox = gCoordinator.GetComponent <CollisionBox> (entity);
		auto& player = gCoordinator.GetComponent <Player> (entity);
		
		
		//center camera to follow all players on screen
		
		//if only 1 player is playing
		if(m_num_players == 1)
		{
			//center camera over the only player
			camera_rect_ptr->x = ( transform.position.x + (collisionBox.width / 2) ) - (camera_rect_ptr->width / 2);
			camera_rect_ptr->y = ( transform.position.y + (collisionBox.height / 2) ) - (camera_rect_ptr->height / 2);
			
			
		}
		//else if more than 1 player is playing
		else if(m_num_players > 1)
		{
			//if this is the first player, initialze farthest left and farthest right
			if(player.player_num == 1 && player.alive)
			{
				farthest_left = transform.position.x;
				farthest_right = transform.position.x + 10;
			}
			
			//if a player is more left than the current farthest left
			if(transform.position.x - (collisionBox.width / 2) < farthest_left
			  && transform.position.x - (collisionBox.width / 2) > boundLeft)
			{
				farthest_left = transform.position.x - (collisionBox.width / 2);
				far_left_player_width = collisionBox.width;
			}
			else if(transform.position.x - (collisionBox.width / 2) < boundLeft)
			{
				farthest_left = boundLeft;	
			}
			//if a player is more right than the current fartherst right
			else if(transform.position.x + (collisionBox.width / 2) > farthest_right
			  && transform.position.x + (collisionBox.width / 2) < boundRight)
			{
				farthest_right = transform.position.x + (collisionBox.width / 2);
				far_right_player_width = collisionBox.width;
			}
			else if(transform.position.x + (collisionBox.width / 2) > boundRight)
			{				
				farthest_right = boundRight;
			}
		}
		
		
	}
	
	//if more than 1 player
	if(m_num_players > 1)
	{
		std::cout << "farthest left:" << farthest_left << std::endl;
		std::cout << "farthest right:" << farthest_right << std::endl;
		
		//calculate center point between players
		//float center_x = farthest_left + ( (farthest_right - farthest_left) / 2);
		float center_x = 0.5*(farthest_left + farthest_right);
		float center_y = 180;
		
		std::cout << "center x:" << center_x << std::endl;
		
		float dist_between_players = abs(farthest_right - farthest_left) - far_right_player_width;
		
		float zoom_factor = farthest_right / 640;
		
		zoom_factor = round(zoom_factor / 0.001f)*0.001f;
		
		camera_rect_ptr->width = (1 - zoom_factor)*640;
		if(camera_rect_ptr->width < 640){camera_rect_ptr->width = 640;}
		if(camera_rect_ptr->width > 670 ){camera_rect_ptr->width = 670;}
		
		camera_rect_ptr->height = (1 - zoom_factor)*360;
		
		
		if(camera_rect_ptr->height < 360){camera_rect_ptr->height = 360;}
		if(camera_rect_ptr->height > 390){camera_rect_ptr->height = 390;}
		
		//camera_rect_ptr->x = ( center_x + (far_left_player_width / 2) )*(1 + zoom_factor)*640 - (camera_rect_ptr->width / 2);
		camera_rect_ptr->x = ( center_x - (far_left_player_width / 2) ) - (camera_rect_ptr->width / 2);
		
		//camera_rect_ptr->y = ( center_y + (player_height / 2) )*(1 + zoom_factor)*360 - (camera_rect_ptr->height / 2);
		camera_rect_ptr->y = ( center_y + (player_height / 2) ) - (camera_rect_ptr->height / 2);
		
		//Keep the camera in bounds
		if( camera_rect_ptr->x < 0 )
		{ 
			camera_rect_ptr->x = 0;
		}
		
		if( camera_rect_ptr->y < 0 )
		{
			camera_rect_ptr->y = 0;
		}
		
		
		if( camera_rect_ptr->x > boundRight - camera_rect_ptr->width/(1 + zoom_factor) )
		{
			camera_rect_ptr->x = boundRight - camera_rect_ptr->width/(1 + zoom_factor);
		}
		
		if( camera_rect_ptr->y > boundDown - camera_rect_ptr->height/(1 + zoom_factor))
		{
			camera_rect_ptr->y = boundDown - camera_rect_ptr->height/(1 + zoom_factor);
		}
		
		
		scale = std::min((float)GetScreenWidth()/camera_rect_ptr->width, (float)GetScreenHeight()/camera_rect_ptr->height);
		
		std::cout << "\nCamera System\n";
		
		std::cout << "camera x: " << camera_rect_ptr->x << std::endl;
		std::cout << "camera y: " << camera_rect_ptr->y << std::endl;
		std::cout << "camera width: " << camera_rect_ptr->width << std::endl;
		std::cout << "camera height: " << camera_rect_ptr->height << std::endl;
		//std::cout << "dist between players: " << dist_between_players << std::endl;
		//std::cout << "zoom factor: " << zoom_factor << std::endl;
		//std::cout << "scale factor: " << scale << std::endl;
		
		
		
	}
	
	
	
	//Keep the camera in bounds
	if( camera_rect_ptr->x < 0 )
	{ 
		camera_rect_ptr->x = 0;
	}
	
	if( camera_rect_ptr->y < 0 )
	{
		camera_rect_ptr->y = 0;
	}
	
	if( camera_rect_ptr->x > boundRight - camera_rect_ptr->width )
	{
		camera_rect_ptr->x = boundRight - camera_rect_ptr->width;
	}
	
	if( camera_rect_ptr->y > boundDown - camera_rect_ptr->height )
	{
		camera_rect_ptr->y = boundDown - camera_rect_ptr->height;
	}
	
	
	
	
	//std::cout << "\nIn Camera System\n";
	//std::cout << "camera position: " << camera_rect_ptr->x << " , " << camera_rect_ptr->y << std::endl;
	 
	
}

void CameraSystem::update_timer(float& dt){zoom_time_counter += dt;}
