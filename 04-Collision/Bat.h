#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CBat : public CGameObject
{
	float drop; 
public:
	CBat()
	{
		AddAnimation("bat_ani_idle");
		AddAnimation("bat_ani_flying");
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	
	void SetDrop(float drop) { this->drop = this->y; }
};

