#include "HUD.h"
#include"Game.h"
#include"Sprites.h"

void HUD::Update()
{
	auto simon = CPlayScene::GetInstance()->GetSimon();
	simonHp = simon->GetHp();

	//unsigned int score = CSimon::GetScore();
	//string scoreTempt;
	//if (score < 10)
	//{
	//	scoreTempt = "00000" + std::to_string(score);
	//}
	//else if (score < 100)
	//{
	//	scoreTempt = "0000" + std::to_string(score);
	//}
	//else if (score_ < 1000) {
	//	scoreTempt = "000" + std::to_string(score);
	//}
	//else if (score_ < 10000)
	//{
	//	scoreTempt = "00" + std::to_string(score);
	//}
	//else if (score_ < 100000)
	//{
	//	scoreTempt = "0" + std::to_string(score);
	//}
	//else {
	//	scoreTempt = std::to_string(score);
	//}


	//_UIinfor = "SCORE-" + score + " TIME 0" + std::to_string(scene->GetTime()) + " STAGE 0" + std::to_string(this->state) + "\n";
	//_UIinfor = _UIinfor + "PLAYER                  -" + enery_ + "\n";
	//_UIinfor += "ENEMY                   -02\n";
//}
}

void HUD::Render()
{	
	CGame::GetInstance()->DrawUIText(this->information, rect);
	CSprites::GetInstance()->Get("blackBoard")->DrawHud(297,33);
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
}

