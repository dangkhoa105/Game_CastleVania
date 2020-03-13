#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

#include "Whip.h"

class CSimon : public CGameObject
{
	int level;
	int untouchable;

	DWORD untouchable_start;
public:
	//bool isFight = false;
	//bool isJumping = false;
	//bool isSit = false;
	bool isGround = false;

	bool isComplete = false;

	CSimon() : CGameObject()
	{
		//level = SIMON_LEVEL_BIG;
		untouchable = 0;

		ReadResourceFile* readResourceFile = ReadResourceFile::GetInstance();

		vector<string> animationsSimon = readResourceFile->GetAnimations("resources\\Castlevania\\castlevania_ani.xml");
		for each (string animation in animationsSimon)
		{
			AddAnimation(animation);
		}
	}
	CWhip *whip =new CWhip();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	//void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	int GetY() { return y; }
	int GetX() { return x; }
	int GetVy() { return vy; }

	//bool IsJumping();
	//bool IsGround();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool IsHitting();

	bool IsComplete();
};