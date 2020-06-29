#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CMonkey : public CEnemy
{
	DWORD jump_start = 0;
	DWORD onGround_start = 0;
	bool isJumping = false;
	bool isJumpingActive = false;
public:
	CMonkey()
	{
		hp = 1;
		AddAnimation("monkey_ani_idle");
		AddAnimation("monkey_ani_jumping");
		this->SetState(MONKEY_STATE_IDLE);
		this->jump_start = 0;
		this->onGround_start = 0;
		reSpawnWaitingTime = ENEMY_SPAWN_TIME;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);

	DWORD GetJumpStartTime() { return this->jump_start; }
	void ResetJumpStartTime() { this->jump_start = 0; }
};

