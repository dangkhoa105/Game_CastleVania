#include "PlayScene.h"
#include <dinput.h>
#include "debug.h"
#include "Game.h"

void PlayScene::LoadResource()
{
	CGame* game = CGame::GetInstance();
	game = CGame::GetInstance();
	CTextures* textures = CTextures::GetInstance();
	textures->Add(400, L"resources\\Map\\map1.png", D3DCOLOR_XRGB(255, 0, 255));
	auto texMap = textures->Get(400);
	tilemap = new CTileMap(L"resources\\Map\\map1.txt", 1536, 320, 32, 32);
	tilemap->SetTileMap(texMap);
	tilemap->LoadResources();

	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	ReadResourceFile* ReadResourceFile = ReadResourceFile::GetInstance();

	ReadResourceFile->LoadAllResource();

	LPANIMATION ani;

	//load objects here
	const std::string path = "resources\\CastleVania\\map1_objects.xml";
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("objects");

	for (xml_node<>* child = rootNode->first_node(); child; child = child->next_sibling()) //cú pháp lập
	{
		const std::string nodeName = child->name();
		if (nodeName == "candles")
		{
			for (xml_node<>* grand = child->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				CCandle* candle = new CCandle();
				const int x = std::atoi(grand->first_attribute("x")->value());
				const int y = std::atoi(grand->first_attribute("y")->value());
				const int itemID = std::atoi(grand->first_attribute("itemId")->value());
				candle->SetItem(itemID);
				candle->SetPosition(x,y);
				objects.push_back(candle);
			}
		}
	}

	// simon
	simon = new CSimon();
	simon->SetPosition(100, 200);
	objects.push_back(simon);

	// iteam heart
	ani = new CAnimation(100);
	ani->Add("item_heart");
	animations->Add("item_heart", ani);

	// iteam whip
	ani = new CAnimation(100);
	ani->Add("item_whip");
	animations->Add("item_whip", ani);

	// brick
	ani = new CAnimation(100);
	ani->Add("brick");
	animations->Add("brick", ani);

	for (int i = 0; i < 100; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(0 + i * 16.0f, 320 - 32);
		objects.push_back(brick);
	}
}

void PlayScene::Update(DWORD dt)
{
	// We know that Simon is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	simon->whip->Update(dt, &objects);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CCandle*>(objects.at(i)))
		{
			auto f = dynamic_cast<CCandle*>(objects.at(i));

			if (f->IsDestroy())
			{
				float x, y;
				f->GetPosition(x, y);
				if (f->GetItem()!=-1)
				{
					if (f->GetItem() == ID_TEX_IHEART)
					{
						auto heart = new CIHeart();
						heart->SetPosition(x, y);
						objects.push_back(heart);
					}
					if (f->GetItem() == ID_TEX_IWHIP)
					{
						auto whip = new CIWhip();
						whip->SetPosition(x, y);
						objects.push_back(whip);
					}
					if (f->GetItem() == ID_TEX_IKNIFE)
					{
						auto knife = new CIKnife();
						knife->SetPosition(x, y);
						objects.push_back(knife);
					}
				}			
				auto effect = new CEffect();
				effect->SetPosition(x, y);
				objects.push_back(effect);
			}
		}
	}

	// Update camera to follow simon
	if (simon->x > SCREEN_WIDTH / 2) {
		CGame::GetInstance()->SetCamPos(simon->x - SCREEN_WIDTH /*cx*/, 0.0f /*cy*/);
	}

	float cx, cy;
	simon->GetPosition(cx, cy);

	if (cx > SCREEN_WIDTH / 2) {
		CGame::GetInstance()->SetCamPos(cx - SCREEN_WIDTH / 2, 0.0f /*cy*/);
	}

	for (vector<LPGAMEOBJECT>::iterator it = objects.begin(); it != objects.end(); ) {

		if ((*it)->IsDestroy()) {
			it = objects.erase(it);
		}
		else ++it;
	}
}

void PlayScene::Render()
{
	CGame* game = CGame::GetInstance();
	tilemap->Draw(game->GetCamPos());
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isDestroy)
		{
			return;
		}
		objects[i]->Render();
	}
}

void PlayScene::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	if ((simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_IDLE) && simon->isGround == false) return;
	//simon->GetUpgradeTime() tương đương  simon->GetUpgradeTime()!=0
	if (simon->GetUpgradeTime() && GetTickCount() - simon->GetUpgradeTime() > SIMON_UPGRADE_TIME) 
	{
		simon->SetState(SIMON_STATE_IDLE); 
		simon->ResetUpgradeTime();
	} 

	if (simon->GetAttackTime() && GetTickCount() - simon->GetAttackTime() > SIMON_ATTACK_TIME)
	{
		
		simon->ResetAttack();
		simon->SetState(SIMON_STATE_IDLE);
	}

	if (simon->GetAttackTime())
	{
		return;
	}

	if (simon->GetUpgradeTime())
	{
		return;
	}

	if (simon->GetState() == SIMON_STATE_ITEM)
	{
		return;
	}
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT)) {
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		//if (simon->IsHitting() == true && simon->isGround == false) return;
		if (simon->isGround == true)
		simon->SetState(SIMON_STATE_SIT); 
	}
	else {		
		simon->SetState(SIMON_STATE_IDLE); 
		
	}
}

void PlayScene::OnKeyDown(int KeyCode)
{
	if (simon->GetState() == SIMON_STATE_ITEM) return;
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	if (simon->GetAttackTime())
	{
		return;
	}

	if (KeyCode == DIK_SPACE) {
		if (simon->isGround == false || simon->IsHitting() == true) return;
		simon->SetState(SIMON_STATE_JUMP);
	}
	else if (KeyCode == DIK_Z) {
		
		if (simon->GetAttackTime() == 0 && simon->isKnife == false)
		{
			simon->isKnife = false;
			if (simon->GetState() == SIMON_STATE_SIT) simon->SetState(SIMON_STATE_SIT_ATTACK);
			else if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_JUMP) simon->SetState(SIMON_STATE_STAND_ATTACK);
		}
	}
	else if (KeyCode == DIK_C) {
	
		if (simon->GetAttackTime() == 0 && simon->isKnife == true)
		{
			simon->isKnife = true;
			if (simon->GetState() == SIMON_STATE_SIT) simon->SetState(SIMON_STATE_SIT_ATTACK);
			else if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_JUMP) simon->SetState(SIMON_STATE_STAND_ATTACK);
		}
	}
}

