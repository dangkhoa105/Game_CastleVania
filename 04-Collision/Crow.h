#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CCrow : public CEnemy
{
	DWORD flyingWaiting_start = 0;
	
	bool isWaiting = true;
public:
	DWORD flying_start = 0;
	CCrow()
	{
		hp = 1;
		AddAnimation("crow_ani_idle");
		AddAnimation("crow_ani_flying");
		reSpawnTime = ENEMY_SPAWN_TIME;
		this->SetState(CROW_STATE_IDLE);
		this->flyingWaiting_start = 0;
		this->flying_start = 0;
	};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
};

