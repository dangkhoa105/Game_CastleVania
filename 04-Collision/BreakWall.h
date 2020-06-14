#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBreakWall : public CGameObject
{
	CBreakWall() : CGameObject()
	{
		AddAnimation("big_candle");
		AddAnimation("small_candle");
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};

