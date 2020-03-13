#pragma once

#pragma region WINDOW
#define WINDOW_CLASS_NAME	L"Castlevania"
#define MAIN_WINDOW_TITLE	L"Castle Vania"

#define BACKGROUND_COLOR	D3DCOLOR_XRGB(255, 255, 255)
#define SCREEN_WIDTH		512
#define SCREEN_HEIGHT		480

#define DEFAULT_CELL_WIDTH		256
#define DEFAULT_CELL_HEIGHT		240

#define MAX_FRAME_RATE			120
#pragma endregion

#pragma region ID TEXTURE
#define ID_TEX_SIMON		0
#define ID_TEX_BRICK		10
#define ID_TEX_WHIP			20
#define ID_TEX_CANDLE		30

#define ID_TEX_BBOX			-100		// special texture to draw object bounding box
#pragma endregion


#pragma region SIMON
#define SIMON_WALKING_SPEED			0.15f 
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_JUMP_DEFLECT_SPEED	0.2f
#define SIMON_GRAVITY				0.002f
#define SIMON_DIE_DEFLECT_SPEED		0.5f

#define SIMON_STATE_IDLE				0
#define SIMON_STATE_WALKING_RIGHT		100
#define SIMON_STATE_WALKING_LEFT		200
#define SIMON_STATE_JUMP				300
#define SIMON_STATE_SIT					400
#define SIMON_STATE_STAND_ATTACK		500
#define SIMON_STATE_SIT_ATTACK			600
#define SIMON_STATE_JUMP_ATTACK			700
#define SIMON_STATE_DIE					800

#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1
#define SIMON_ANI_WALKING_RIGHT			2
#define SIMON_ANI_WALKING_LEFT			3
#define SIMON_ANI_JUMP					4
#define SIMON_ANI_SIT					5
#define SIMON_ANI_STAND_ATTACK			6
#define SIMON_ANI_SIT_ATTACK			7

#define SIMON_BBOX_WIDTH			30
#define SIMON_BBOX_HEIGHT			60

#define SIMON_UNTOUCHABLE_TIME		5000	
#pragma endregion

#pragma region BRICK
#define BRICK_BBOX_WIDTH			32
#define BRICK_BBOX_HEIGHT			32
#pragma endregion

#pragma region CANDLE
#define CANDLE_BBOX_WIDTH			32
#define CANDLE_BBOX_HEIGHT			64

#define SMALL_CANDLE_BBOX_WIDTH		16
#define SMALL_CANDLE_BBOX_HEIGHT	32
#pragma endregion

#pragma region WEAPONS
// Whip
#define WHIP_BBOX_WIDTH				32
#define WHIP_BBOX_HEIGHT			32

#define WHIP_STATE_IDLE				1
#define WHIP_STATE_FIGHT			2
#pragma endregion