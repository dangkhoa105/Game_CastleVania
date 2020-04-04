#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CEffect : public CGameObject
{
public:
	CEffect() : CGameObject()
	{
		ReadResourceFile* readResourceFile = ReadResourceFile::GetInstance();

		vector<string> animations = readResourceFile->GetAnimations("resources\\Effect\\Effect_ani.xml");
		for each (string animation in animations)
		{
			AddAnimation(animation);
		}
	}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

