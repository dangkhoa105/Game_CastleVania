#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CZombie : public CEnemy
{	
public:
	CZombie()
	{
		hp = 1;
		AddAnimation("zombie_ani_walking");
		this->SetState(ZOMBIE_STATE_IDLE);
		reSpawnWaitingTime = ENEMY_SPAWN_TIME;
	};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
};

