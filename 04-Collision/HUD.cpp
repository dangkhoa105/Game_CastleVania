#include "HUD.h"
#include "Game.h"
#include "Sprites.h"

void HUD::Update()
{
	auto simon = CPlayScene::GetInstance()->GetSimon();;
	heart = simon->GetHeart() < 10 ? "0" + std::to_string(simon->GetHeart()) : std::to_string(simon->GetHeart());
	simonHp = simon->GetHp();

	unsigned int score = simon->GetScore();
	string scoreTempt;
	if (score < 10)
	{
		scoreTempt = "00000" + std::to_string(score);
	}
	else if (score < 100)
	{
		scoreTempt = "0000" + std::to_string(score);
	}
	else if (score < 1000) 
	{
		scoreTempt = "000" + std::to_string(score);
	}
	else if (score < 10000)
	{
		scoreTempt = "00" + std::to_string(score);
	}
	else if (score < 100000)
	{
		scoreTempt = "0" + std::to_string(score);
	}
	else 
	{
		scoreTempt = std::to_string(score);
	}

	information = "SCORE-" + scoreTempt + " TIME 0" + std::to_string(CPlayScene::GetInstance()->GetGameTime()) + " SCENE 0" + std::to_string(CPlayScene::GetInstance()->GetCurrentScene()) + "\n";
	information = information + "PLAYER                  -" + heart + "\n";
	information += "ENEMY                   -02\n";
}

void HUD::Render()
{
	CGame::GetInstance()->DrawUIText(this->information, rect);
	CSprites::GetInstance()->Get("blackBoard")->DrawHud(297, 33);
	auto simon = CPlayScene::GetInstance()->GetSimon();

	switch (simon->subWeapons)
	{
	case SUBWEAPON::KNIFE:
		CSprites::GetInstance()->Get("item_knife")->DrawHud(312, 44);
		break;
	case SUBWEAPON::AXE:
		CSprites::GetInstance()->Get("axe_1")->DrawHud(314, 39);
		break;
	case SUBWEAPON::BOOMERANG:
		CSprites::GetInstance()->Get("boomerang_1")->DrawHud(314, 40);
		break;
	case SUBWEAPON::CLOCK:
		CSprites::GetInstance()->Get("clock")->DrawHud(314, 36);
		break;
	case SUBWEAPON::GAS:
		CSprites::GetInstance()->Get("item_gas")->DrawHud(314, 38);
		break;
	default:
		break;
	}

	for (int i = 0; i < 16; i++)
		if (i < simonHp)
			CSprites::GetInstance()->Get("simonHp")->DrawHud(109 + i * 9, 34, 255);
		else
			CSprites::GetInstance()->Get("subHp")->DrawHud(109 + i * 9, 34, 255);

	for (int i = 0; i < 16; i++)
		if (i < bossHp)
			CSprites::GetInstance()->Get("bossHp")->DrawHud(109 + i * 9, 52, 255);
		else
			CSprites::GetInstance()->Get("subHp")->DrawHud(109 + i * 9, 52, 255);

	if (simon->isDoubleShot)
		CSprites::GetInstance()->Get("item_double_shot")->DrawHud(464, 40, 255);
}
