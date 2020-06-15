#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"
	
class CSpearGuard : public CEnemy
{	
	int beginPositionX;
	int lastPositionX;

	RECT activeBox;
public:
	CSpearGuard()
	{
		AddAnimation("spear_guard_ani");	
		hp = 2;
		this->SetState(SPEAR_GUARD_STATE_WALKING);
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);

	void SetReturnPosition(int bX, int lX);
};

