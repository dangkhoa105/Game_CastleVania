#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBreakWall : public CGameObject
{
public:
	CBreakWall() : CGameObject()
	{
		AddAnimation("breakWall_idle");
		//AddAnimation("breakWall_destroyed");
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};

