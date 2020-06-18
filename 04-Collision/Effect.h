#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

#define EFFECT_TIME 500

#define EFFECT_STATE_CANDLE		0
#define EFFECT_STATE_ENEMIES	1
#define EFFECT_STATE_BREAKWALL	2
#define EFFECT_STATE_BREAKWALL_DESTROYED	3

class CEffect : public CGameObject
{
	DWORD life_start; // bắt đầu đếm khi tạo ra effect
public:	
	CEffect() : CGameObject()
	{
		life_start = GetTickCount();
		AddAnimation("effect");
		AddAnimation("effect_enemies_die");
		AddAnimation("breakWall_breaking");
		AddAnimation("breakWall_destroyed");
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

