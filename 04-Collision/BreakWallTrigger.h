#pragma once
#include "GameObject.h"
#include "Define.h"

class CBreakWallTrigger : public CGameObject
{
public:
	CBreakWallTrigger() {};
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};

