#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBoss : public CEnemy
{
	DWORD attack_start;
	float attack_time = 0;
	DWORD waiting_start;
	float waiting_time = 0;
	DWORD flyBack_start;
	float flyBack_time = 0;
	bool outOfArea = false;
	bool flyRandom = false;

	D3DXVECTOR2 simonPos = { 0, 0 };

	bool awake = false;

	DWORD untouchable_start;
public:
	CBoss()
	{
		hp = 16;
		AddAnimation("boss_ani_idle");
		AddAnimation("boss_ani_flying");
		this->SetState(BOSS_STATE_IDLE);
		attack_start = 0;
		waiting_start = 0;
		flyBack_start = 0;
		reSpawnWaitingTime = ENEMY_SPAWN_TIME;
	}

	int GetHp() { return this->hp; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxAwake(float& left, float& top, float& right, float& bottom);
};

