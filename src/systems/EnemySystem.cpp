#include "EnemySystem.h"


#include "core/coordinator.h"


extern Coordinator gCoordinator;


void EnemySystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& enemy = gCoordinator.GetComponent<Enemy>(entity);
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		
		
		switch(enemy.type)
		{
			case EnemyType::NONE:{break;}
			case EnemyType::SKELETON:
			{
				
				break;
			}
			default:{break;}
		}
		
		// Forces
		//auto const& gravity = gCoordinator.GetComponent<Gravity2D>(entity);
		
		//move transform component by velocity of rigid body multiplied by time
		//transform.position.x += rigidBody.velocity.x * dt;
		//transform.position.y += rigidBody.velocity.y * dt;
		
		//account for acceleration due to gravity to rigid body velocity
		//rigidBody.velocity.x += gravity.force.x * dt;
		//rigidBody.velocity.y += gravity.force.y * dt;
	}
}
