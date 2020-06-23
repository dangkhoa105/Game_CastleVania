#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBoss : public CEnemy
{
	DWORD attack_time;
	DWORD waiting_time;
	DWORD fly_time;

	D3DXVECTOR2 simonPos = { 0, 0 };

	bool awake = false;
public:
	CBoss()
	{
		AddAnimation("boss_ani_idle");
		AddAnimation("boss_ani_flying");
		this->SetState(BOSS_STATE_IDLE);
		attack_time = 0;
		waiting_time = 0;
		fly_time = 0;
	}

	void Reset()
	{

	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxAwake(float& left, float& top, float& right, float& bottom);
};

