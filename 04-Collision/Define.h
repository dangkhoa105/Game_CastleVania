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
#define SIMON_WALKING_SPEED				0.13f 
#define SIMON_JUMP_SPEED_Y				0.45f
#define SIMON_JUMP_DEFLECT_SPEED		0.2f
#define SIMON_GRAVITY					0.0015f
#define SIMON_DIE_DEFLECT_SPEED			0.5f

#define SIMON_STATE_IDLE				0
#define SIMON_STATE_WALKING_RIGHT		10
#define SIMON_STATE_WALKING_LEFT		20
#define SIMON_STATE_JUMP				30
#define SIMON_STATE_SIT					40
#define SIMON_STATE_STAND_ATTACK		50
#define SIMON_STATE_SIT_ATTACK			60
#define SIMON_STATE_ITEM				70
#define SIMON_STATE_AUTO_WALKING		80
#define SIMON_STATE_DIE					90

#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1
#define SIMON_ANI_WALKING_RIGHT			2
#define SIMON_ANI_WALKING_LEFT			3
#define SIMON_ANI_JUMP					4
#define SIMON_ANI_SIT					5
#define SIMON_ANI_STAND_ATTACK			6
#define SIMON_ANI_SIT_ATTACK			7
#define SIMON_ANI_ITEM					8

#define SIMON_BBOX_WIDTH				30
#define SIMON_BBOX_HEIGHT				60

#define SIMON_UNTOUCHABLE_TIME			5000	
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

#pragma region BRICK
#define BRICK_BBOX_WIDTH				32
#define BRICK_BBOX_HEIGHT				32
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
#pragma endregion

#pragma region ITEMS
#define GRAVITY_ITEM					0.0003f

#define ID_IHEART						1
#define ID_IWHIP						2
#define ID_IKNIFE						3
#define ID_IMONEYBAG					4

// Heart
#define IHEART_BBOX_WIDTH				16
#define IHEART_BBOX_HEIGHT				16

// Whip
#define IWHIP_BBOX_WIDTH				32
#define IWHIP_BBOX_HEIGHT				32

// Knife
#define IKNIFE_BBOX_WIDTH				32
#define IKNIFE_BBOX_HEIGHT				16

// Money Bag
#define IMONEYBAG_BBOX_WIDTH			31
#define IMONEYBAG_BBOX_HEIGHT			30
#pragma endregion