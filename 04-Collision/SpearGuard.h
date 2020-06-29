#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"
	
class CSpearGuard : public CEnemy
{	
public:
	CSpearGuard()
	{
		AddAnimation("spear_guard_ani");	
		hp = 2;
		this->SetState(SPEAR_GUARD_STATE_WALKING);
	}

	int GetHp() { return this->hp; };

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
};

