#include "HUD.h"
#include"Game.h"
#include"Sprites.h"
void HUD::Update()
{
}

void HUD::Render()
{
	//animations["blackBoard_ani"]->Render(330, 42);
	
	
	CGame::GetInstance()->DrawUIText(this->information, rect);
	CSprites::GetInstance()->Get("blackBoard")->DrawHud(300,36);

	auto simon = CPlayScene::GetInstance()->GetSimon();

	if (simon->subWeapons==SUBWEAPON::KNIFE)
	{
		CSprites::GetInstance()->Get("item_knife")->DrawHud(330, 42);
	}

	//for (int i = 0; i < 16; i++)
	//	if (i < 16)
	//		animations["simonHp_ani"]->Render(130 + 12 * i, 42);//42
	//	else
	//		animations["subHp_ani"]->Render(130 + 12 * i, 42);

	//for (int i = 0; i < 16; i++)
	//	if (i < 16)
	//		animations["bossHp_ani"]->Render(130 + 12 * i, 68); //68
	//	else
	//		animations["subHp_ani"]->Render(130 + 12 * i, 68);
}

