#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CKnife : public CGameObject
{
public:
	CKnife() : CGameObject()
	{
		AddAnimation("item_knife");

		SetState(KNIFE_STATE_FIGHT);

	}

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);

	void SetState(int state);
};

