#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBoss : public CEnemy
{
	float drop;
public:
	CBoss()
	{
		AddAnimation("boss_ani_idle");
		AddAnimation("boss_ani_flying");
		this->SetState(GHOST_STATE_IDLE);
		this->nx = -1;
		reSpawnTime = ENEMY_SPAWN_TIME;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
};

