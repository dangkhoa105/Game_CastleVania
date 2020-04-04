#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"
#include "Effect.h"

class CCandle : public CGameObject
{
	int id;
	bool destroyed;
public:
	CEffect* effect = new CEffect();

	CCandle() : CGameObject()
	{
		ReadResourceFile* readResourceFile = ReadResourceFile::GetInstance();

		vector<string> animationsWhip = readResourceFile->GetAnimations("resources\\Castlevania\\castlevania_ani.xml");
		for each (string animation in animationsWhip)
		{
			AddAnimation(animation);

		}

		vector<string> animationsCandleDestroyed = readResourceFile->GetAnimations("resources\\Effect\\Effect_ani.xml");
		for each (string animation in animationsCandleDestroyed)
		{
			AddAnimation(animation);
		}
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void SetState(int state);
	//bool IsDestroy();
};

