#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBat : public CEnemy
{
	float drop; 
	DWORD attack_start = 0;
	float time_x = 0;
public:
	CBat()
	{
		hp = 1;
		AddAnimation("bat_ani_idle");
		AddAnimation("bat_ani_flying");
		reSpawnTime = 3000;
		this->SetState(BAT_STATE_IDLE);
	};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
	
	void SetDrop() { this->drop = this->y; }
};

