#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"
#include "Simon.h"

class CWall : public CGameObject
{
public:
	CSimon* simon;
	CWall()
	{
		AddAnimation("wall");
	}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

