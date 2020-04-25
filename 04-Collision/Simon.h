#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

#include "Whip.h"
#include "Knife.h"

enum class SUBWEAPON {
	DEFAULT,
	KNIFE,
};

class CSimon : public CGameObject
{
	int level = 1;
	int untouchable;
	int idChangeScene;

	DWORD untouchable_start;

	DWORD update_start;
	DWORD attack_start;
	DWORD entrace_start;

public:
	bool isGround = false;	
	bool isComplete = false;
	bool isKnife = false;
	bool spawnKnife = false;

	SUBWEAPON subWeapon;

	DWORD GetUpgradeTime() { return this->update_start; }
	void ResetUpgradeTime() { this->update_start = 0; }

	DWORD GetAttackTime() { return this->attack_start; }
	void ResetAttackTime() { this->attack_start = 0; }

	DWORD GetEntraceTime() { return this->entrace_start; }
	void ResetEntraceTime() { this->entrace_start = 0; }
	
	void ResetAttack()
	{
		whip->ResetAttack();
		animations["simon_ani_sit_attacking"]->ResetAnimation();
		animations["simon_ani_attacking"]->ResetAnimation();
		animations["simon_ani_walking"]->ResetAnimation();
		this->attack_start = 0;
	}

	void ResetEntrace()
	{
		this->entrace_start = 0;
	}

	CSimon() : CGameObject()
	{
		whip = new CWhip();
		knife = new CKnife();

		untouchable = 0;
		update_start = 0;
		attack_start = 0;
		entrace_start = 0;

		subWeapon = SUBWEAPON::DEFAULT;

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
	//int GetIdChangeScene() { return idChangeScene; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool IsHitting();

	//bool IsComplete();
};