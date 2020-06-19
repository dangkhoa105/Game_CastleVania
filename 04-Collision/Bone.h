#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBone : public CGameObject
{
public:
	CBone() : CGameObject()
	{
		AddAnimation("bone_ani");
		this->vx = BONE_SPEED_VX;
		this->vy = -BONE_SPEED_VY;
	}

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);

	void SetState(int state);
};

