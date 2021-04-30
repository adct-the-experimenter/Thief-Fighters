#include "core/coordinator.h"
#include "raylib.h"

#include "systems/InputReactorSystem.h"

#include "systems/PhysicsSystem.h"

#include "systems/CameraSystem.h"
#include "systems/RenderSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/AttackPowerMechanicSystem.h"
#include "systems/PlayerDeathSystem.h"
#include "core/ControllerInputHandler.h"
#include "core/ControllerInput.h"

#include "core/KeyboardTypingInputHandler.h"
#include "core/KeyboardInput.h"

#include "misc/camera.h"
#include "misc/MediaLoader.h"
#include "misc/globalvariables.h"

#include "misc/StageManager.h"

#include "misc/char_selector.h" //for CharacterSelector class
#include "misc/stage_selector.h" //for StageSelector class
#include "misc/num_player_setter.h" //for NumPlayerSetter class


#include <string>
#include <chrono>
#include <memory>
#include <iostream>
#include <array>

//main

//initialize manager for levels
//initialize list of levels from XML

	
Coordinator gCoordinator;

ControllerInput gControllerInput;
ControllerInputHandler gControllerInputHandler;

KeyboardInput gKeyboardInput;
KeyboardTypingInputHandler gKeyboardTypingInputHandler;

NumPlayerSetter gNumPlayerSetter;

std::vector <Entity> entities(MAX_ENTITIES);

//function to initialize main ECS
void InitMainECS();

std::shared_ptr <RenderSystem> renderSystem;
std::shared_ptr <AnimationSystem> animationSystem;

std::shared_ptr <InputReactorSystem> input_ReactSystem;


std::shared_ptr <PhysicsSystem> physicsSystem;

std::shared_ptr <AttackPowerMechanicSystem> attackPowerMechanicSystem;

std::shared_ptr <PlayerDeathSystem> playerDeathSystem;

//function to init raylib system
void InitRaylibSystem();

//function to close raylib system
void CloseRaylibSystem();

//function to load media for game
bool loadMedia();
MediaLoader gMediaLoader;

//function to run the game loop of event handling, logic render, sound
void GameLoop();

//game loop functions
void handle_events(); //receive input
void logic(); //determine what happens in world based on input
void render(); //draw visual representation of what happens in world to screen
void sound(); //play sounds of audio representation of what happens in world 

//game state
enum class GameState : std::uint8_t {TITLE_MENU=0, CHAR_SELECTOR, STAGE_SELECTOR, FIGHT_GAME};
GameState m_game_state = GameState::TITLE_MENU;

//camera to follow players.
std::shared_ptr <CameraSystem> cameraSystem;
CustomCamera main_camera;

bool video_game_playing = false;

CharacterSelector gCharSelector;
std::int8_t gNumPlayers = 2;

const std::int16_t screenWidth = 800;
const std::int16_t screenHeight = 600;

StageManager gStageManager;

StageSelector gStageSelector;

int main(int argc, char* args[])
{
	InitRaylibSystem();
	
	
	if(!loadMedia())
	{
		std::cout << "Not loading game. Failed to load media!\n";
	}
	else
	{
	
		//reset to new number of players for input handling
		gControllerInput.Init(gNumPlayers);
		gControllerInputHandler.Init(gNumPlayers);
		
		gNumPlayerSetter.Init();
		
		InitMainECS();
		
		
		
		
		bool quit = false;

		while (!quit)
		{
			// Detect window close button or ESC key
			if(WindowShouldClose())
			{
				quit = true;
			}    
			
			// Main game loop
						
			GameLoop();
		}
							
	}
	
	gMediaLoader.freeMedia();
	gStageManager.FreeCurrentLoadedLevel();
    
	CloseRaylibSystem();
	
	return 0;
}

void GameLoop()
{
	//handle events through event manager
	handle_events();
	
	//run logic for all entities through systems
	logic();
	
	//run render for all entities in manager
	render();
	
}

void handle_events()
{
	gControllerInputHandler.Update(&gControllerInput);
	
	gKeyboardTypingInputHandler.Update(&gKeyboardInput);
	
	switch(m_game_state)
	{
		case GameState::TITLE_MENU:
		{
			gNumPlayerSetter.handle_input(gControllerInput,gKeyboardInput);
			break;
		}
		case GameState::CHAR_SELECTOR:
		{
			//run logic for character creator system here
			gCharSelector.handle_input(gControllerInput,gKeyboardInput);
			break;
		}
		case GameState::STAGE_SELECTOR:
		{
			gStageSelector.handle_input(gControllerInput,gKeyboardInput);
			break;
		}
		case GameState::FIGHT_GAME:
		{
			input_ReactSystem->Update(gControllerInput);
			break;
		}
	}
}

void logic()
{
	
	float dt = GetFrameTime();
	
	//game
	
	switch(m_game_state)
	{
		case GameState::TITLE_MENU:
		{
			bool moveNextState = false;
			
			gNumPlayerSetter.logic();
			
			//if need to move to next state
			if(gNumPlayerSetter.MoveToNextStateBool())
			{
				moveNextState = true;
				
				gNumPlayers = gNumPlayerSetter.GetNumberOfPlayers();
				
			}
			
			
			//if need to move to next state
			if(moveNextState)
			{
				//reset to new number of players for input handling
				gControllerInput.Init(gNumPlayers);
				gControllerInputHandler.Init(gNumPlayers);				
				
				//create entities for players
				for(size_t i = 0; i < gNumPlayers; i++)
				{
					entities[i] = gCoordinator.CreateEntity();
				}
				
				//initialze char selector
				gCharSelector.Init(&entities,gNumPlayers);
				
				//initialize camera system
				cameraSystem->Init(&main_camera,screenWidth,screenHeight);
				
				//initialize render system
				renderSystem->Init(&main_camera);
				
				//move to next state
				m_game_state = GameState::CHAR_SELECTOR;
			}
			
			break;
		}
		case GameState::CHAR_SELECTOR:
		{
			//run logic for character selector
			gCharSelector.logic();
			
			if(gCharSelector.MoveToNextStateBool())
			{
				//initialize stage selector
				gStageSelector.Init(gNumPlayers);
				
				m_game_state = GameState::STAGE_SELECTOR;
			}
			
			break;
		}
		case GameState::STAGE_SELECTOR:
		{
			//run logic for stage selector
			gStageSelector.logic();
			
			if(gStageSelector.MoveToNextStateBool())
			{
				//initialize fight game state
				
				if(gStageManager.LoadLevel( gStageSelector.StageSelected() ) )
				{
					m_game_state = GameState::FIGHT_GAME;
				}
				
				gStageManager.PlacePlayersInStage(gNumPlayers);
				attackPowerMechanicSystem->Init(gNumPlayers);
			}
			
			break;
		}
		case GameState::FIGHT_GAME:
		{
			attackPowerMechanicSystem->Update(dt);
			
			physicsSystem->Update(dt);
			
			//set up frame for render
			animationSystem->Update(dt);
			
			//check for dead players, set bool to stop rendering them
			playerDeathSystem->Update();
			
			break;
		}
	}
}

Vector3 mapPosition = { -16.0f, 0.0f, -8.0f };  // Set model position

void render()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);
	
	
	switch(m_game_state)
	{
		case GameState::TITLE_MENU:
		{
			DrawTexture(title_menu_texture, 0, 0, WHITE);
			
			gNumPlayerSetter.render();
			
			break;
		}
		case GameState::CHAR_SELECTOR:
		{
			DrawText("In character selector. Press A to select character.", 80, 20, 20, BLACK);
			gCharSelector.render();
			break;
		}
		case GameState::STAGE_SELECTOR:
		{
			DrawText("In stage selector. Press A to select stage.", 80, 20, 20, BLACK);
			gStageSelector.render();
			break;
		}
		case GameState::FIGHT_GAME:
		{			
			
			//draw the stage
			DrawTexture(main_stage.texture, 0, 0, RAYWHITE);
			
			//cameraSystem->Update();
			
		    //render any entity that has render component
			renderSystem->Update();
			
						
			break;
		}
	}
	
	
	
	EndDrawing();
}

void sound()
{
	
}

bool loadMedia()
{
	bool success = true;
	
	if( !gMediaLoader.loadMedia() )
	{
		success = false;
	}
	
	return success;
}

void InitMainECS()
{
	//initialize coordinator which initializes entity manager, component manager
	gCoordinator.Init();
	
	//Initialize components for entities
	
	gCoordinator.RegisterComponent<Transform2D>(); //id 1
	gCoordinator.RegisterComponent<RigidBody2D>();
	gCoordinator.RegisterComponent<RenderModelComponent>();
	gCoordinator.RegisterComponent<InputReact>();
	gCoordinator.RegisterComponent<PhysicsTypeComponent>();
	gCoordinator.RegisterComponent<Animation>();
	gCoordinator.RegisterComponent<CollisionBox>();
	gCoordinator.RegisterComponent<Player>();
	
	
	//make rendering system that only reacts to entities
	//with render info component
	renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
	
	
	Signature sig_render;
	sig_render.set(gCoordinator.GetComponentType<RenderModelComponent>());
	sig_render.set(gCoordinator.GetComponentType<Transform2D>());
	gCoordinator.SetSystemSignature<RenderSystem>(sig_render);
	
	//make input reactor system that only reacts to components input react and transform
	input_ReactSystem = gCoordinator.RegisterSystem<InputReactorSystem>();
	
	Signature sig_input_react;
	sig_input_react.set(gCoordinator.GetComponentType<InputReact>());
	sig_input_react.set(gCoordinator.GetComponentType<RigidBody2D>());
	sig_input_react.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<InputReactorSystem>(sig_input_react);
	
	//make physics system that only reacts to entitities 
	//with signature that has these components
	
	gCoordinator.RegisterComponent<Gravity2D>(); 
	physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
	
	
	Signature phys_sys_signature;
	phys_sys_signature.set(gCoordinator.GetComponentType<Gravity2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<RigidBody2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<Transform2D>());
	phys_sys_signature.set(gCoordinator.GetComponentType<PhysicsTypeComponent>());
	phys_sys_signature.set(gCoordinator.GetComponentType<CollisionBox>());
	gCoordinator.SetSystemSignature<PhysicsSystem>(phys_sys_signature);
	
	//make camera system that only reacts to entities
	//with signature that has these components
	
	cameraSystem = gCoordinator.RegisterSystem<CameraSystem>();
	
	Signature camera_sig;
	camera_sig.set(gCoordinator.GetComponentType<Transform2D>());
	camera_sig.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<CameraSystem>(camera_sig);
	
	
	//make animation system that only reacts to entities
	//with signature that has these components
	
	animationSystem = gCoordinator.RegisterSystem<AnimationSystem>();
	animationSystem->Init();
	
	Signature animation_sig;
	animation_sig.set(gCoordinator.GetComponentType<Transform2D>());
	animation_sig.set(gCoordinator.GetComponentType<Animation>());
	animation_sig.set(gCoordinator.GetComponentType<RenderModelComponent>());
	gCoordinator.SetSystemSignature<AnimationSystem>(animation_sig);
	
	//make special power mechanic system
	
	attackPowerMechanicSystem = gCoordinator.RegisterSystem<AttackPowerMechanicSystem>();
	
	Signature special_power_mechanic_sig;
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<Player>());
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<Transform2D>());
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<Animation>());
	gCoordinator.SetSystemSignature<AttackPowerMechanicSystem>(special_power_mechanic_sig);
	
	
	//make player death system
	
	playerDeathSystem = gCoordinator.RegisterSystem<PlayerDeathSystem>();
	
	Signature player_death_sig;
	player_death_sig.set( gCoordinator.GetComponentType<Player>() );
	player_death_sig.set( gCoordinator.GetComponentType<RenderModelComponent>() );
	gCoordinator.SetSystemSignature<PlayerDeathSystem>(player_death_sig);
	
	
}

void InitRaylibSystem()
{
	
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation
	
    InitWindow(screenWidth, screenHeight, "Thief Fighters");
	
	
	// initialize SDL2 for gamepad handling
	if( SDL_Init( SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0 )
	{
		printf( "SDL input could not initialize! SDL Error: %s\n", SDL_GetError() );
	}

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    
    //initialize game controller input
    gControllerInputHandler.Init(1);
}

void CloseRaylibSystem()
{
    CloseWindow();        // Close window and OpenGL context
    
    //Quit SDL subsystems
    SDL_Quit();
}

