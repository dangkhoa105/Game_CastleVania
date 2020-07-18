#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

#define EFFECT_TIME 500

#define EFFECT_STATE_CANDLE					0
#define EFFECT_STATE_ENEMIES				1
#define EFFECT_STATE_BREAKWALL				2
#define EFFECT_STATE_BREAKWALL_DESTROYED	3
#define EFFECT_STATE_MONEYBAG_SPECIAL		4
#define EFFECT_STATE_MONEYBAG_400			5
#define EFFECT_STATE_MONEYBAG_700			6
#define EFFECT_STATE_MONEYBAG_1000			7
#define EFFECT_STATE_2000					8

class CEffect : public CGameObject
{
	DWORD life_start; // bắt đầu đếm khi tạo ra effect
public:	
	CEffect() : CGameObject()
	{
		life_start = 0;
		AddAnimation("effect");
		AddAnimation("effect_enemies_die");
		AddAnimation("breakWall_breaking");
		AddAnimation("breakWall_destroyed");
		AddAnimation("effect_moneybag_special");
		AddAnimation("effect_moneybag_400");
		AddAnimation("effect_moneybag_700");
		AddAnimation("effect_moneybag_1000");
		AddAnimation("effect_2000");
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

