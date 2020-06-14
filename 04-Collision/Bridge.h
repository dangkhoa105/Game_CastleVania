#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBridge : public CGameObject
{
	int beginPositionX;
	int lastPositionX;
public:
	CBridge() : CGameObject()
	{
		AddAnimation("bridge");
		vx = BRIDGE_SPEED;
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void SetReturnPosition(int bX, int lX) { this->beginPositionX = bX; this->lastPositionX = lX; }
};

