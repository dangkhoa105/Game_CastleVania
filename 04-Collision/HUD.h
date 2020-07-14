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
	string heart;
	int time;
	int subWeapon;
public:
	HUD()
	{
		CAnimations::GetInstance()->Get("blackBoard_ani");
		CAnimations::GetInstance()->Get("simonHp_ani");
		CAnimations::GetInstance()->Get("bossHp_ani");
		CAnimations::GetInstance()->Get("subHp_ani");

		SetRect(&rect, 10, 16, SCREEN_WIDTH, SCREEN_HEIGHT);
		information = "SCORE-000000 TIME 0000 SCENE 00\n";
		information += " PLAYER                  -00\n";
		information += "ENEMY                   -00\n";

		bossHp = 16;
	};

	void Update();
	void Render();
	void SetBossHp(int hp) { this->bossHp = hp; }
};

