#pragma once
#include "GameObject.h"
#include "Define.h"

class CEntrace : public CGameObject
{
public:
	CEntrace() {};
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};

