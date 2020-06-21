#pragma once

#pragma region WINDOW
#define WINDOW_CLASS_NAME	L"Castlevania"
#define MAIN_WINDOW_TITLE	L"Castle Vania"

#define BACKGROUND_COLOR	D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH			527
#define SCREEN_HEIGHT			501

#define DEFAULT_CELL_WIDTH		256
#define DEFAULT_CELL_HEIGHT		240

#define MAX_FRAME_RATE			120
#pragma endregion

#pragma region ID TEXTURE
#define ID_TEX_CASTLEVANIA		0

#define ID_TEX_BBOX				-100		// special texture to draw object bounding box
#pragma endregion

#pragma region SCENE
#define ID_SCENE_1						0
#define ID_SCENE_2						1
#define ID_SCENE_3						2
#define ID_SCENE_4						3
#pragma endregion

#pragma region EFFECT
#define EFFECT							1
#pragma endregion

#pragma region SIMON
#define SIMON_ATTACK_TIME				350
#define SIMON_UPGRADE_TIME				500
#define SIMON_WALKING_SPEED				0.12f 
#define SIMON_JUMP_SPEED_Y				0.45f
#define SIMON_JUMP_DEFLECT_SPEED		0.2f
#define SIMON_GRAVITY					0.0013f
#define SIMON_DIE_DEFLECT_SPEED			0.5f
#define SIMON_WALKING_STAIR_SPEED		0.079f
#define SIMON_HURT_SPEED_X				0.2f
#define SIMON_HURT_SPEED_Y				0.35f

#define SIMON_STATE_IDLE					0
#define SIMON_STATE_WALKING_RIGHT			1
#define SIMON_STATE_WALKING_LEFT			2
#define SIMON_STATE_JUMP					3
#define SIMON_STATE_SIT						4
#define SIMON_STATE_STAND_ATTACK			5
#define SIMON_STATE_SIT_ATTACK				6
#define SIMON_STATE_ITEM					7
#define SIMON_STATE_AUTO_WALKING			8
#define SIMON_STATE_AUTO_WALKING_STAIR		9
#define SIMON_STATE_STAIR_UP_IDLE			10
#define SIMON_STATE_STAIR_UP_RIGHT			11
#define SIMON_STATE_STAIR_UP_LEFT			12
#define SIMON_STATE_ATTACK_STAIR_UP			13
#define SIMON_STATE_STAIR_DOWN_IDLE			14
#define SIMON_STATE_STAIR_DOWN_RIGHT		15
#define SIMON_STATE_STAIR_DOWN_LEFT			16
#define SIMON_STATE_ATTACK_STAIR_DOWN		17
#define SIMON_STATE_HURT					18
#define SIMON_STATE_DIE						19

#define SIMON_BBOX_WIDTH					30
#define SIMON_BBOX_HEIGHT					60

#define SIMON_UNTOUCHABLE_TIME				5000	
#pragma endregion

#pragma region ENEMY
#define ENEMY_SPAWN_TIME				5000
// Spear Guard
#define SPEAR_GUARD_WALKING_SPEED		0.05f

#define SPEAR_GUARD_STATE_DIE			0
#define SPEAR_GUARD_STATE_WALKING		1
#define SPEAR_GUARD_CHASE_SIMON         2

// Bat
#define BAT_FLYING_SPEED_X				0.3f
#define BAT_FLYING_SPEED_Y				0.02f
#define BAT_DROP						30

#define BAT_STATE_IDLE					0
#define BAT_STATE_FLYING				1

// Ghost
#define GHOST_FLYING_SPEED_X			0.06f
#define GHOST_DROP						20

#define GHOST_STATE_IDLE				0
#define GHOST_STATE_FLYING				1

// Monkey
#define MONKEY_JUMPING_SPEED			0.4f
#define MONKEY_GRAVITY					0.003f

#define MONKEY_STATE_IDLE				0
#define MONKEY_STATE_START				1
#define MONKEY_STATE_JUMPING			2

// Skeleton
#define SKELETON_JUMPING_SPEED_X		0.1f
#define SKELETON_GRAVITY				0.003f

#define SKELETON_STATE_IDLE				0
#define SKELETON_STATE_JUMPING			1

// crow
#define CROW_FLYING_SPEED_X				0.15f
#define CROW_FLYING_SPEED_Y				0.2f

#define CROW_STATE_IDLE					0
#define CROW_STATE_WAITING				1
#define CROW_STATE_FLYING				2

// boss
#define BOSS_FLYING_SPEED_X				0.15f
#define BOSS_FLYING_SPEED_Y				0.2f

#define BOSS_STATE_IDLE					0
#define BOSS_STATE_WAITING				1
#define BOSS_STATE_FLYING				2
#pragma endregion

#pragma region BREAK WALL
#define BREAKWALL_BREAKING_GRAVITY		0.0005f

#define BREAKWALL_BBOX_WIDTH			32
#define BREAKWALL_BBOX_HEIGHT			32

#define BREAKWALLTRIGGER_BBOX_WIDTH		30
#define BREAKWALLTRIGGER_BBOX_HEIGHT	8

#define BREAKWALL_STATE_IDLE			0
#define BREAKWALL_STATE_DESTROYED		1
#pragma endregion

#pragma region CHANGE SCENE
#define CHANGESCENE_BBOX_WIDTH			1
#define CHANGESCENE_BBOX_HEIGHT			8

#define CHANGESCENE_STATE_IDLE			1
#define CHANGESCENE_STATE_DESTROY		2
#pragma endregion

#pragma region ENTRACE
#define ENTRACE_BBOX_WIDTH				1
#define ENTRACE_BBOX_HEIGHT				8
#pragma endregion

#pragma region MONEY BAG TRIGGER
#define MONEYBAG_BBOX_WIDTH				75
#define MONEYBAG_BBOX_HEIGHT			10

#define MONEYBAG_STATE_IDLE				0
#define MONEYBAG_STATE_TOUCH			1
#pragma endregion

#pragma region STAIR
#define STAIR_BBOX_WIDTH				32
#define STAIR_BBOX_HEIGHT				4
#pragma endregion

#pragma region CANDLE
#define CANDLE_STATE_BIG				0
#define CANDLE_STATE_SMALL				1
#define CANDLE_STATE_DESTROYED			2

#define CANDLE_BBOX_WIDTH				32
#define CANDLE_BBOX_HEIGHT				64

#define SMALL_CANDLE_BBOX_WIDTH			16
#define SMALL_CANDLE_BBOX_HEIGHT		32
#pragma endregion

#pragma region BRIDGE
#define BRIDGE_SPEED					0.05f

#define BRIDGE_BBOX_WIDTH				64
#define BRIDGE_BBOX_HEIGHT				16
#pragma endregion

#pragma region WEAPONS
// Whip
#define WHIP_BBOX_WIDTH					32
#define WHIP_BBOX_HEIGHT				32

#define WHIP_STATE_IDLE					1
#define WHIP_STATE_FIGHT				2

// Knife
#define KNIFE_VX						0.5f

#define KNIFE_BBOX_WIDTH				32
#define KNIFE_BBOX_HEIGHT				16

#define KNIFE_STATE_IDLE				1
#define KNIFE_STATE_FIGHT				2

// Bone
#define BONE_GRAVITY					0.001f
#define BONE_SPEED_VX					0.3f
#define BONE_SPEED_VY					0.4f

#define BONE_BBOX_WIDTH					32
#define BONE_BBOX_HEIGHT				32
#pragma endregion

#pragma region ITEMS
#define GRAVITY_ITEM					0.0003f

#define ID_IHEART						1
#define ID_SMALL_IHEART					2
#define ID_IWHIP						3
#define ID_IKNIFE						4
#define ID_IMONEYBAG					5
#define ID_IBOOMERANG					6
#define ID_ICROWN						7
#define ID_IDOUBLESHOT					8

// Heart
#define IHEART_BBOX_WIDTH				16
#define IHEART_BBOX_HEIGHT				16

#define ISMALLHEART_BBOX_WIDTH			8
#define ISMALLHEART_BBOX_HEIGHT			8

// Whip
#define IWHIP_BBOX_WIDTH				32
#define IWHIP_BBOX_HEIGHT				32

// Knife
#define IKNIFE_BBOX_WIDTH				32
#define IKNIFE_BBOX_HEIGHT				16

// Money Bag
#define IMONEYBAG_BBOX_WIDTH			31
#define IMONEYBAG_BBOX_HEIGHT			30

// Boomerang
#define IBOOMERANG_BBOX_WIDTH			28
#define IBOOMERANG_BBOX_HEIGHT			28

// Crown
#define ICROWN_BBOX_WIDTH				31
#define ICROWN_BBOX_HEIGHT				31

// Double Shot
#define IDOUBLESHOT_BBOX_WIDTH			27
#define IDOUBLESHOT_BBOX_HEIGHT			27
#pragma endregion