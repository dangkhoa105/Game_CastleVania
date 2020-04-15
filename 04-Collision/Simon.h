#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

#include "Whip.h"
#include "Knife.h"

class CSimon : public CGameObject
{
	int level = 1;
	int untouchable;

	DWORD untouchable_start;

	DWORD update_start;
	DWORD attack_start;

public:
	bool isGround = false;	
	bool isComplete = false;
	bool isKnife = false;

	DWORD GetUpgradeTime() { return this->update_start; }
	void ResetUpgradeTime() { this->update_start = 0; }

	DWORD GetAttackTime() { return this->attack_start; }
	void ResetAttackTime() { this->attack_start = 0; }

	void ResetAttack()
	{
		whip->ResetAttack();
		animations["simon_ani_sit_attacking"]->ResetAnimation();
		animations["simon_ani_attacking"]->ResetAnimation();
		this->attack_start = 0;
	}

	CSimon() : CGameObject()
	{
		whip = new CWhip();
		knife = new CKnife();
		untouchable = 0;
		update_start = 0;
		attack_start = 0;

		AddAnimation("simon_ani_idle");
		AddAnimation("simon_ani_walking");
		AddAnimation("simon_ani_jumping");
		AddAnimation("simon_ani_sitting");
		AddAnimation("simon_ani_attacking");
		AddAnimation("simon_ani_sit_attacking");
		AddAnimation("simon_ani_attacking_lv");
		AddAnimation("simon_ani_item");
	}

	CWhip* whip;
	CKnife* knife;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	int GetY() { return y; }
	int GetX() { return x; }
	int GetVy() { return vy; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool IsHitting();

	//bool IsComplete();
};