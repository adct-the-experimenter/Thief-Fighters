
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




struct AnimationInfo
{
	int animsCount = 0;
    ModelAnimation *anims = nullptr;
    int animFrameCounter = 0;
	
};

struct Animation
{
	AnimationInfo info;
};


struct RenderModelComponent
{
    Texture2D texture;
};

struct Player
{
	std::uint8_t player_num;
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
