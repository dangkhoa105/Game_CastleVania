#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBat : public CEnemy
{
	float drop; 
	DWORD attack_start;
	float time_x = 0;
public:

	D3DXVECTOR2 simonPos = { 0, 0 };
	CBat()
	{
		hp = 1;
		AddAnimation("bat_ani_idle");
		AddAnimation("bat_ani_flying");
		reSpawnWaitingTime = ENEMY_SPAWN_TIME;
		this->SetState(BAT_STATE_IDLE);
		attack_start = 0;
	};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
	
	void SetDrop() { this->drop = this->y; }

	void SetTimeX(float time_x) { this->time_x = time_x; }
	float GetTimeX() { return this->time_x; }

	void SetAttackTime(DWORD time) { this->attack_start = time; }
	DWORD GetAttackTime() { return this->attack_start; }
};

