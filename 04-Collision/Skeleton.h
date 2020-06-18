#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"
#include "Bone.h"

class CSkeleton : public CEnemy
{
	DWORD onGround_start = 0;
public:
	CSkeleton()
	{
		AddAnimation("skeleton_ani_jumping");
		DWORD onGround_start = 0;
		this->state = SKELETON_STATE_IDLE;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
};

