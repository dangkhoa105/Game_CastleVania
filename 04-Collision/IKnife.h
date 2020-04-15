#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CIKnife : public CGameObject
{
public:
	CIKnife()
	{
		AddAnimation("item_knife");
	}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void SetState(int state);
};

