#pragma once
#include "GameObject.h"
#include "Define.h"

class Heart : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void SetState(int state);
};

