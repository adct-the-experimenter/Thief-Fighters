
#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <bitset>
#include <array>
#include <vector>
#include "raylib.h" //for vector2

// A simple type alias
using ComponentType = std::uint8_t;

// Used to define the size of arrays later on
const ComponentType MAX_COMPONENTS = 34;

struct Gravity2D
{
	Vector2 force;
};


struct RigidBody2D
{
	Vector2 velocity;
	Vector2 acceleration;
};


struct Transform2D
{
	Vector2 position;
	Vector2 rotation;
	Vector2 scale;
};


struct CollisionBox
{
	std::uint32_t width;
	std::uint32_t height;
};

enum class AnimatedActorType : std::int8_t {NONE=0,PLAYER,ENEMY};

struct Animation
{
	AnimatedActorType anim_actor_type; 
	
	//position of frame and frame direction
	
	//for general movement
	std::uint16_t horiz_frame_offset = 0;
	
	//for specific movement i.e. walk(0), normal attack(1), special attack(2-9)
	std::uint16_t vert_frame_offset = 0;
	
	
	std::uint16_t frame_width = 30;
	std::uint16_t frame_height = 60;
	std::uint8_t frame_count = 0;
	
	//for movement based animation
	Vector2 last_position;
	
	//attack mode, indicates which attack mode a character is in
	//for player, -1 = none, 0 = normal attack, 1-9 = special attack
	std::int8_t attackMode = -1;
	
	bool facing_left = false; 
};


struct RenderModelComponent
{
	Vector2 position;
	Texture2D* texture_ptr;
	Rectangle frame_rect;
	Color tint;
	bool render;
};

struct AttackBox
{
	bool active = false;
	std::uint8_t player_num;
	Rectangle collisionBox;
	
};

struct Player
{
	std::uint8_t player_num;
	
	//making health signed to avoid health becoming large if it ends up being less than zero.
	std::int16_t player_health;
	
	bool alive;
	
	//parameters related to power
	
	//indicate if player pressed power button
	bool powerButtonPressed;
	
	//indicate if player pressed regular attack button
	bool regularAttackButtonPressed;
	
	//bitset to indicate which power a player has
	std::bitset <8> collected_powers;
	
	//bitset to indicate which power a player has activated
	std::bitset <8> powers_activated;
	
	//cooldown timer for regular arract
	float regular_attack_cooldown_timer_val;
	
	//cooldown timers for each special power
	std::array <float,8> sp_attack_cooldown_timer_val_array;
	
	//current power set to be used
	std::uint8_t current_power; //0-7
	
	//power requested by player input
	std::int8_t requested_power; //-1,0-7
	
	//indicate which player last hit this player
	std::uint8_t last_hit_by_player_num;
	
	//attack collision box
	AttackBox attack_box;
	
	//bool to indicate if player is suffering damage already.
	bool taking_damage;
	
	
};

enum class InputReactorType : std::uint8_t { NONE=0, PLAYER, CAR};
struct InputReact
{
	InputReactorType actor_type;
	bool reactToInput = false;
	std::uint8_t player_num = 0;
};

enum class PhysicsType : std::uint8_t {LIFE_RPG=0, PLATFORMER, FIGHTING_GAME};
struct PhysicsTypeComponent
{
	PhysicsType phy_type;
};

enum class EnemyType : std::uint8_t {NONE=0, SKELETON};
struct Enemy
{
	EnemyType type;
};

#endif
