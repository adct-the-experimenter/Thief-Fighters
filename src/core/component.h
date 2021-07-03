//    MIT License

//    Copyright (c) 2020 Austin Morlan

//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is furnished
//    to do so, subject to the following conditions:

//    The above copyright notice and this permission notice (including the next
//    paragraph) shall be included in all copies or substantial portions of the
//    Software.

//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
//    OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
//    OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
	float jump_speed;
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

enum class FaceDirection : std::uint8_t {NONE=0,NORTH,SOUTH,EAST,WEST};

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
	
	//direction that player is facing
	FaceDirection face_dir;
	
	//bool to indicate if player was hurt and needs to be in hurt animation
	bool hurt;
	
	
};


struct RenderModelComponent
{
	Vector2 position;
	//Texture2D* texture_ptr;
	//index to use from array holding character textures
	std::int16_t char_texture_index; 
	Rectangle frame_rect;
	Color tint;
	bool render;
	
	//camera that entity is in
	
};

struct AttackBox
{
	bool active = false;
	std::uint8_t player_num;
	Rectangle collisionBox;
	
};

enum class PlayerState : std::uint8_t { IDLE=0, ATTACKING, HURTING};

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
	std::int8_t requested_power; // -1, 0-7
	
	//indicate which player last hit this player
	std::uint8_t last_hit_by_player_num;
	
	//attack collision box
	AttackBox attack_box;
	
	//bool to indicate if player is suffering damage already.
	bool taking_damage;
	
	//character-specific variables for player
	float attack_box_offset;
	float health_factor;
	float speed_factor;
	float jump_factor;
	float damage_factor;
	
	//frame count for hurt animation
	float hurt_anim_time_count;
	
	//state of player
	PlayerState state;
	
	//bool indicating that player is invincible because in pain right now.
	//done to prevent losing health during after attack an in pain i.e. Invisibility frames
	bool hurt_invincible;
	
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
	bool grounded; //indicates if entity is grounded
	std::uint8_t jump_count;
	std::uint8_t jump_count_limit;
};

enum class EnemyType : std::uint8_t {NONE=0, SKELETON};
struct Enemy
{
	EnemyType type;
};

#endif
