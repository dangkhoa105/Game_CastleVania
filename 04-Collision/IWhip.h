#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CIWhip : public CGameObject
{
public:
	CIWhip() 
	{
		AddAnimation("item_whip");
	}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void SetState(int state);
};

