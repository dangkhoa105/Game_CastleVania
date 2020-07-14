#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

#include "Whip.h"
#include "SubWeapon.h"
#include "ChangeScene.h"

enum class SUBWEAPON {
	DEFAULT,
	KNIFE,
	BOOMERANG,
	AXE,
	CLOCK,
	GAS,
};

enum class STAIRDIRECTION {
	DEFAULT,
	UPRIGHT,
	UPLEFT,
	DOWNRIGHT,
	DOWNLEFT,
	UPLEFTRIGHT,
};

class CSimon : public CGameObject
{
	int level = 1;
	int hp;
	int score;
	int heart;
	int untouchable;


	DWORD untouchable_start;

	DWORD update_start;
	DWORD attack_start;

	bool colBrickSweptAABB;

public:
	bool isGround = false;
	bool isComplete = false;
	bool isSubWeapon = false;
	bool spawnSubWeapon = false;
	bool isAutoWalk = false;
	bool isOnStair = false;
	bool isStartOnStair = false;
	bool isColliWithStair = false;
	bool isFirstStepOnStair = false;
	bool isUpStair = false;
	bool isDoubleShot = false;
	bool isColliWithTrigger = false;
	STAIRDIRECTION onStairDirection = STAIRDIRECTION::DEFAULT;
	D3DXVECTOR2 stairPos;
	D3DXVECTOR2 lastStepOnStairPos;
	int idChangeScene = -1;

	SUBWEAPON subWeapons;

	DWORD GetUpgradeTime() { return this->update_start; }
	void ResetUpgradeTime() { this->update_start = 0; }

	DWORD GetAttackTime() { return this->attack_start; }
	void ResetAttackTime() { this->attack_start = 0; }

	//DWORD GetEntraceTime() { return this->entrace_start; }
	//void ResetEntraceTime() { this->entrace_start = 0; }

	void ResetAttack()
	{
		whip->ResetAttack();
		animations["simon_ani_sit_attacking"]->ResetAnimation();
		animations["simon_ani_attacking"]->ResetAnimation();
		animations["simon_ani_walking"]->ResetAnimation();
		animations["simon_ani_attack_stair_up"]->ResetAnimation();
		animations["simon_ani_attack_stair_down"]->ResetAnimation();
		this->attack_start = 0;
	}

	CSimon() : CGameObject()
	{
		hp = 16;
		score = 0;
		heart = 5;
		whip = new CWhip();
		subWeapon = new CSubWeapon();

		untouchable = 0;
		update_start = 0;
		attack_start = 0;

		subWeapons = SUBWEAPON::DEFAULT;

		AddAnimation("simon_ani_idle");
		AddAnimation("simon_ani_hurt");
		AddAnimation("simon_ani_walking");
		AddAnimation("simon_ani_jumping");
		AddAnimation("simon_ani_sitting");
		AddAnimation("simon_ani_attacking");
		AddAnimation("simon_ani_sit_attacking");
		AddAnimation("simon_ani_attacking_lv");
		AddAnimation("simon_ani_stair_up_idle");
		AddAnimation("simon_ani_stair_up");
		AddAnimation("simon_ani_stair_down_idle");
		AddAnimation("simon_ani_stair_down");
		AddAnimation("simon_ani_attack_stair_up");
		AddAnimation("simon_ani_attack_stair_down");
		AddAnimation("simon_ani_item");
		AddAnimation("simon_ani_die");
	}

	CWhip* whip;
	CSubWeapon* subWeapon;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	int GetUntouchable() { return untouchable; }

	int GetY() { return y; }
	int GetX() { return x; }
	int GetVy() { return vy; }

	int GetHp() { return hp; }
	void SetHp() 
	{ 
		hp -= 2;
		if (hp < 0)
			hp = 0;
	}
	int GetScore() { return score; }
	int GetHeart() { return heart; }
	void SetHeart() { heart--; }
	void ResetHPHeart() { hp = 16; heart = 5; }
	//int GetIdChangeScene() { return idChangeScene; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool IsHitting();
	void StandOnStair() { vx = vy = 0; }

	void HandleFirstStepOnStair();
	void HandlePerStepOnStair();
	bool AutoWalk(float step);

	bool CheckIsOnStair() {
		return isOnStair;
	}

	bool CheckColliWithStair() {
		return isColliWithStair;
	}

	bool CheckCanStepUp() {
		if (this->onStairDirection == STAIRDIRECTION::UPLEFT || this->onStairDirection == STAIRDIRECTION::UPRIGHT || this->onStairDirection == STAIRDIRECTION::UPLEFTRIGHT)
			return true;
		return false;
	}

	bool CheckCanStepDown() {
		if (this->onStairDirection == STAIRDIRECTION::DOWNLEFT || this->onStairDirection == STAIRDIRECTION::DOWNRIGHT || this->onStairDirection == STAIRDIRECTION::UPLEFTRIGHT)
			return true;
		return false;
	}

	STAIRDIRECTION CheckStepOnStairDirection() {
		return this->onStairDirection;
	}
	void SetOnStairDirection(STAIRDIRECTION stairDirection) {
		this->onStairDirection = stairDirection;
	}

	bool CheckStartOnStair() {
		return isStartOnStair;
	}
	void SetStartOnStair() {
		this->isStartOnStair = true;
	}

	bool CheckAutoWalk() {
		return this->isAutoWalk;
	}


};