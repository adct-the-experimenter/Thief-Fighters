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
#include "misc/CharacterAssetManager.h"

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
enum class GameState : std::uint8_t {TITLE_MENU=0, CHAR_SELECTOR, STAGE_SELECTOR, TUTORIAL, FIGHT_GAME};
GameState m_game_state = GameState::TITLE_MENU;

//camera to follow players.
std::shared_ptr <CameraSystem> cameraSystem;
CustomCamera main_camera;

CharacterAssetManager gCharAssetManager;

CharacterSelector gCharSelector;
std::int8_t gNumPlayers = 2;

const std::int16_t screenWidth = 800;
const std::int16_t screenHeight = 450;

StageManager gStageManager;

StageSelector gStageSelector;

bool quitGame = false;

//using render texture of resolution 640x360 to scale it up better by integers.
RenderTexture2D target;
int gameScreenWidth = 640;
int gameScreenHeight = 360;
		
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
		
		
		
		// Render texture initialization, used to hold the rendering result so we can easily resize it
		target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
		SetTextureFilter(target.texture, FILTER_BILINEAR);  // Texture scale filter to use
		SetTextureWrap(target.texture,WRAP_CLAMP);
		
		//set camera
		main_camera.Init(gameScreenWidth,gameScreenHeight);
		main_camera.SetLevelBounds(0,720,0,360);

		while (!quitGame)
		{
			// Detect window close button or ESC key
			if(WindowShouldClose())
			{
				quitGame = true;
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

static bool leave_tutorial = false;
static bool restart_game = false;
static int winning_player = -1;

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
		case GameState::TUTORIAL:
		{
			if(gControllerInput.gamepads_vec[0].button_up_released == SDL_CONTROLLER_BUTTON_A)
			{
				leave_tutorial = true;
			}
			break;
		}
		case GameState::FIGHT_GAME:
		{
			input_ReactSystem->Update(gControllerInput);
			
			if(winning_player != -1)
			{
				if(gControllerInput.gamepads_vec[winning_player].button_up_released == SDL_CONTROLLER_BUTTON_START)
				{
					std::cout << "Game restart called!\n";
					restart_game = true;
				}
			}
			
			break;
		}
	}
}

static bool show_restart_game_message = false;

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
				
				gNumPlayerSetter.Reset();
				
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
				
				cameraSystem->Init(&main_camera,gNumPlayers);
				
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
				//load media for requested characters
				RequestedCharacters& req_char = gCharSelector.GetRequestedCharacters();
				if(!gCharAssetManager.LoadCharacterAssets( req_char, gNumPlayers ) )
				{
					quitGame = true;
				}
				
				//initialize stage selector
				gStageSelector.Init(gNumPlayers);
				
				m_game_state = GameState::STAGE_SELECTOR;
				
				gCharSelector.Reset();
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
					m_game_state = GameState::TUTORIAL;
				}
				
				gStageManager.PlacePlayersInStage(gNumPlayers);
				attackPowerMechanicSystem->Init(gNumPlayers);
				playerDeathSystem->Init(gNumPlayers);
			}
			
			break;
		}
		case GameState::TUTORIAL:
		{
			if(leave_tutorial)
			{
				leave_tutorial = false;
				m_game_state = GameState::FIGHT_GAME;
			}
			
			break;
		}
		case GameState::FIGHT_GAME:
		{
			//handle activating powers based on input
			attackPowerMechanicSystem->HandlePowerActivation(dt);
			
			//move players and other entities
			physicsSystem->Update(dt);
			
			//move attack boxes with players
			attackPowerMechanicSystem->MoveAttackBoxesWithPlayer(dt);
			
			//check collisions between players
			attackPowerMechanicSystem->CollisionDetectionBetweenPlayers();
			
			//react to collisions
			attackPowerMechanicSystem->ReactToCollisions(dt);
			
			//perform power transactions if needed so that players can
			//receive their slain opponent's power
			attackPowerMechanicSystem->PerformNeededPowerTransactions();
			
			//set up frame for render
			animationSystem->Update(dt);
			
			//check for dead players, set bool to stop rendering them
			playerDeathSystem->Update();
			
			if(playerDeathSystem->OnePlayerWon() && gNumPlayers > 1)
			{
				show_restart_game_message = true;
				winning_player = playerDeathSystem->GetPlayerWhoWon();
			}
			else if(gNumPlayers == 1)
			{
				show_restart_game_message = true;
				winning_player = 0;
			}
			
			if(restart_game)
			{
				//remove entities
				for(std::uint32_t i = 0; i < gNumPlayers; ++i)
				{
					gCoordinator.DestroyEntity(i);
				}
				
				//set game state back to title screen
				m_game_state = GameState::TITLE_MENU;
			}
			
			
			break;
		}
	}
}

Vector3 mapPosition = { -16.0f, 0.0f, -8.0f };  // Set model position

void render()
{
	// Compute required framebuffer scaling
    float scale = std::min((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);
    
	BeginDrawing();
	
	ClearBackground(BLACK);
	
	BeginTextureMode(target);
	
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
			DrawText("In character selector. Move joystick to change to desired choice. Press A/B to confirm choice.", 80, 20, 20, BLACK);
			gCharSelector.render();
			break;
		}
		case GameState::STAGE_SELECTOR:
		{
			DrawText("In stage selector. Press A to select stage.", 80, 20, 20, BLACK);
			gStageSelector.render();
			break;
		}
		case GameState::TUTORIAL:
		{
			DrawTexture( tutorial_texture,0,0,WHITE ); 
			DrawText("Tutorial, Player 1 press A to continue to game.",80,15,15, BLACK);
			break;
		}
		case GameState::FIGHT_GAME:
		{			
			//std::cout << "scale before camera system: " << scale << std::endl;
			
			cameraSystem->Update(scale);
			
			//draw the stage
			
			DrawTextureRec(main_stage.texture, *main_camera.GetCameraRectPointer(), Vector2{0,0}, RAYWHITE);
			//DrawTexture(main_stage.texture, 0,0, RAYWHITE);
			
		    //render any entity that has render component
			renderSystem->Update();
			
			attackPowerMechanicSystem->DebugRender();
			
			if(show_restart_game_message)
			{
				DrawText("Winning player press start to return to title screen to restart game.", 80,15,15, GOLD);
			}
						
			break;
		}
	}
	
	EndTextureMode();
	
	
	// Draw RenderTexture2D to window, properly scaled
	//DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
	//			   (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
	//			   (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
	
	//float rect_x = (float)main_camera.GetCameraRectPointer()->x*scale;
	float rect_x = 0;
	if(rect_x <= 0){rect_x = 0;}
	
	//float rect_y = (float)main_camera.GetCameraRectPointer()->y*scale;
	float rect_y = 0;
	if(rect_y <= 0){rect_y = 0;}
	
	float rect_width = (float)main_camera.GetCameraRectPointer()->width*scale;
	float rect_height = (float)main_camera.GetCameraRectPointer()->height*scale;
	
	//std::cout << "render x: " << rect_x << std::endl;
	//std::cout << "render y: " << rect_y << std::endl;
	//std::cout << "render width: " << rect_width << std::endl;
	//std::cout << "render height: " << rect_height << std::endl;
	
	DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
				   
				   (Rectangle){ rect_x, rect_y, rect_width, rect_height },
				       (Vector2){ 0, 0 }, 0.0f, WHITE);

	EndDrawing();
}

void sound()
{
	
}

bool loadMedia()
{
	
	if( !gMediaLoader.loadMedia() )
	{
		return false;
	}
	
	else if(!gCharAssetManager.LoadCharacterProfilesFromXML())
	{
		return false;
	}
	
	return true;
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
	camera_sig.set(gCoordinator.GetComponentType<CollisionBox>());
	camera_sig.set(gCoordinator.GetComponentType<Player>());
	gCoordinator.SetSystemSignature<CameraSystem>(camera_sig);
	
	
	//make animation system that only reacts to entities
	//with signature that has these components
	
	animationSystem = gCoordinator.RegisterSystem<AnimationSystem>();
	animationSystem->Init();
	
	Signature animation_sig;
	animation_sig.set(gCoordinator.GetComponentType<Transform2D>());
	animation_sig.set(gCoordinator.GetComponentType<RigidBody2D>());
	animation_sig.set(gCoordinator.GetComponentType<Animation>());
	animation_sig.set(gCoordinator.GetComponentType<RenderModelComponent>());
	gCoordinator.SetSystemSignature<AnimationSystem>(animation_sig);
	
	//make special power mechanic system
	
	attackPowerMechanicSystem = gCoordinator.RegisterSystem<AttackPowerMechanicSystem>();
	
	Signature special_power_mechanic_sig;
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<Player>());
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<Transform2D>());
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<RigidBody2D>());
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<Animation>());
	special_power_mechanic_sig.set(gCoordinator.GetComponentType<CollisionBox>());
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
	// Window configuration flags
	// Set MSAA 4X hint before windows creation
	// set v sync
	//set resizeable window
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);  
    
	
    InitWindow(screenWidth, screenHeight, "Thief Fighters");
	SetWindowMinSize(0.5*gameScreenWidth, 0.5*gameScreenHeight);
	
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
	UnloadRenderTexture(target);    // Unload render texture
	
    CloseWindow();        // Close window and OpenGL context
    
    //Quit SDL subsystems
    SDL_Quit();
}

