#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CSubWeapon : public CGameObject
{	
	int damage = 1;
	bool isFight = false;
	bool fight;

	DWORD freeze_start;

	bool isBurning = false;
	DWORD burning_start;
public:
	
	CSubWeapon() : CGameObject()
	{
		AddAnimation("boomerang_ani");
		AddAnimation("axe_ani");
		AddAnimation("item_knife");
		AddAnimation("gas_ani_idle");
		AddAnimation("gas_ani_burning");
		isFight = true;
		freeze_start = GetTickCount();
		burning_start = GetTickCount();
	}

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	void SetState(int state);

	bool CheckIsFight() { return this->isFight; }
	void SetIsFight(bool isFight) { this->isFight = isFight; }

	void SetFight() { this->fight = true; }
};

