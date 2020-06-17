#pragma once
#include "Enemy.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CMonkey : public CEnemy
{
	DWORD jump_start = 0;
public:
	CMonkey()
	{
		AddAnimation("monkey_ani_idle");
		AddAnimation("monkey_ani_jumping");
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom);
};

