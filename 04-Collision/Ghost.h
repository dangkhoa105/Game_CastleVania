#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CGhost : public CEnemy
{
	float drop;
public:
	CGhost()
	{
		AddAnimation("ghost_ani_flying");
		this->SetState(GHOST_STATE_IDLE);
		this->nx = -1;
		drop = 289;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);

	void SetDrop() { this->drop = this->y; }
};

