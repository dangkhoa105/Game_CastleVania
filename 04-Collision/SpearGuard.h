#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CSpearGuard : public CGameObject
{	
public:
	CSpearGuard()
	{
		AddAnimation("spear_guard_ani");
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void SetState(int state);
};

