#pragma once
#include "d3dx9.h"
#include "Define.h"
#include <string>
#include "Animations.h"

class HUD
{
	RECT rect;
	std::string information;
	unordered_map<string, LPANIMATION> animations;

	int simonHp;
	int bossHp;
	int score;
	int time;
	int subWeapon;
public:
	HUD()
	{
		CAnimations::GetInstance()->Get("blackBoard_ani");
		CAnimations::GetInstance()->Get("simonHp_ani");
		CAnimations::GetInstance()->Get("bossHp_ani");
		CAnimations::GetInstance()->Get("subHp_ani");

		SetRect(&rect, 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT);
		information = "SCORE-000000 TIME 0000 SCENE 00\n";
		information += "PLAYER                  -00\n";
		information += "ENEMY                   -00\n";
	};

	void Update();
	void Render();
};

