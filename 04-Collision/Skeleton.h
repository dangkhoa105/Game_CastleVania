#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"
#include "Bone.h"

class CSkeleton : public CEnemy
{
	DWORD onGround_start = 0;
	DWORD throw_start = 0;
	D3DXVECTOR2 simonPos = { 0, 0 };
public:
	CSkeleton()
	{
		hp = 1;
		AddAnimation("skeleton_ani_jumping");
		onGround_start = 0;
		throw_start = 0;
		this->SetState(SKELETON_STATE_IDLE);
		reSpawnWaitingTime = ENEMY_SPAWN_TIME;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxChangeState(float& left, float& top, float& right, float& bottom);
};

