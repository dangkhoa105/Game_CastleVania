#include "PlayScene.h"
#include <dinput.h>
#include "debug.h"
#include "Game.h"

void CPlayScene::LoadResource()
{
	CGame* game = CGame::GetInstance();

	LoadMap();

	// simon
	simon = new CSimon();
	simon->SetPosition(100, 200);
	objects.push_back(simon);

	LoadItem();

	LPANIMATION ani;

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

	for (int i = 0; i < 2; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(-32 , 182 + i * 16.0f);
		objects.push_back(brick);
	}
	

	// entrance
	CEntrace* entrace = new CEntrace();
	entrace->SetPosition(1375, 274);
	objects.push_back(entrace);

	//// change scene
	//CChangeScene* changeScene = new CChangeScene();
	//changeScene->SetPosition(1376, 274);
	//objects.push_back(changeScene);

	// money bag trigger
	CMoneyBagTrigger* moneyBagTrigger = new CMoneyBagTrigger();
	moneyBagTrigger->SetPosition(1402, 274);
	objects.push_back(moneyBagTrigger);
}

void CPlayScene::LoadMap()
{
	// load texture, animation
	ReadResourceFile->LoadAllResource();

	const std::string pathMap = "resources\\CastleVania\\castlevania.xml";
	rapidxml::file<> xmlFileMap(pathMap.c_str());
	rapidxml::xml_document<> docMap;
	docMap.parse<0>(xmlFileMap.data());
	xml_node<>* rootNodeMap = docMap.first_node("base");

	int i = 0;
	int activeScene = 0;
	for (xml_node<>* child = rootNodeMap->first_node(); child; child = child->next_sibling()) {
		const std::string nodeName = child->name();
		if (nodeName == "maps")
		{
			for (xml_node<>* grand = child->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				const int texID = std::atoi(grand->first_attribute("texId")->value());
				const int id = std::atoi(grand->first_attribute("id")->value());
				const int tileWidth = std::atoi(grand->first_attribute("tileWidth")->value());
				const int tileHeight = std::atoi(grand->first_attribute("tileHeight")->value());
				const int mapWidth = std::atoi(grand->first_attribute("mapWidth")->value());
				const int mapHeight = std::atoi(grand->first_attribute("mapHeight")->value());
				std::string tempPathText = std::string(grand->first_attribute("pathText")->value());
				wstring path_objectText(tempPathText.begin(), tempPathText.end());
				LPCWSTR pathText = path_objectText.c_str();

				std::string tempPathImg = std::string(grand->first_attribute("pathImg")->value());
				wstring path_objectImg(tempPathImg.begin(), tempPathImg.end());
				LPCWSTR pathImg = path_objectImg.c_str();


				textures->Add(texID, pathImg, D3DCOLOR_XRGB(255, 0, 255));
				auto texMap = textures->Get(texID);

				CTileMap* tilemap = new CTileMap(pathText, mapWidth, mapHeight, tileWidth, tileHeight, id);
				//tilemap = new CTileMap(L"resources\\Map\\map2.txt", 1024, 768, 32, 32);
				tilemap->SetTileMap(texMap);
				tilemap->LoadResources();

				gameMaps.Add(tilemap);
			}

		}
		else if (nodeName == "scenes")
		{
			activeScene = std::atoi(child->first_attribute("activeID")->value());

			for (xml_node<>* grand = child->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				const int id = std::atoi(grand->first_attribute("id")->value());
				const int mapID = std::atoi(grand->first_attribute("mapId")->value());
				CPScene* pScene = new CPScene();
				pScene->id = id;
				pScene->mapId = mapID;

				pScenes.insert(std::make_pair(id, pScene));
			}
		}
	}

	this->currentScene = pScenes.at(activeScene);

	//this->currentScene = pScenes.at(simon->GetIdChangeScene());

	this->tilemap = this->gameMaps.Get(this->currentScene->mapId);
}

void CPlayScene::LoadItem()
{
	//load objects item
	const std::string path = "resources\\CastleVania\\castlevania.xml";
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
				candle->SetPosition(x, y);
				objects.push_back(candle);
			}
		}
	}
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Simon is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	if (simon->spawnKnife)
	{
		switch (simon->subWeapon)
		{
		case SUBWEAPON::KNIFE:
			auto knife = new CKnife();
			knife->nx = simon->nx;
			if (simon->GetState() == SIMON_STATE_SIT_ATTACK)
				knife->SetPosition(simon->x, simon->y + 30);
			else 
				knife->SetPosition(simon->x, simon->y + 10);
			this->objects.push_back(knife);
			break;
		}
		simon->spawnKnife = false;
	}

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

	UpdateItem();

	UpdateCam();

	Unload();
}

void CPlayScene::Render()
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
	
	if (simon->GetState() == SIMON_STATE_AUTO_WALKING && simon->GetEntraceTime() && GetTickCount() - simon->GetEntraceTime() > SIMON_ATTACK_TIME)
	{
		simon->ResetEntrace();
		simon->SetDestroy();		
	}
}

void CPlayScene::Unload()
{
	for (vector<LPGAMEOBJECT>::iterator it = objects.begin(); it != objects.end(); ) {

		if ((*it)->IsDestroy()) {
			it = objects.erase(it);
		}
		else ++it;
	}
}

void CPlayScene::UpdateCam()
{
	float cx, cy;
	simon->GetPosition(cx, cy);

	// camera
	if (simon->x >= SCREEN_WIDTH / 2 && simon->x < tilemap->GetMapWidth() - SCREEN_WIDTH / 2)
	{
		cx -= SCREEN_WIDTH / 2;
		CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	}
}

void CPlayScene::UpdateItem()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CCandle*>(objects.at(i)))
		{
			auto f = dynamic_cast<CCandle*>(objects.at(i));

			if (f->IsDestroy())
			{
				float x, y;
				f->GetPosition(x, y);
				if (f->GetItem() != -1)
				{
					if (f->GetItem() == ID_IHEART)
					{
						auto heart = new CIHeart();
						heart->SetPosition(x, y);
						objects.push_back(heart);
					}
					if (f->GetItem() == ID_IWHIP)
					{
						auto whip = new CIWhip();
						whip->SetPosition(x, y);
						objects.push_back(whip);
					}
					if (f->GetItem() == ID_IKNIFE)
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

	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CMoneyBagTrigger*>(objects.at(i)))
		{
			auto f = dynamic_cast<CMoneyBagTrigger*>(objects.at(i));
			if (f->IsDestroy())
			{
				auto moneyBag = new CIMoneyBag();
				moneyBag->SetPosition(1207, 240);
				objects.push_back(moneyBag);
			}
		}
	}
}

void CPlayScene::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	if ((simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_IDLE) && simon->isGround == false) 
		return;

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

	if (simon->GetAttackTime() || simon->GetUpgradeTime())
		return;

	if (simon->GetState() == SIMON_STATE_AUTO_WALKING && simon->GetEntraceTime() && GetTickCount() - simon->GetEntraceTime() > SIMON_ATTACK_TIME)
		return;

	if (simon->GetState() == SIMON_STATE_ITEM || simon->GetState() == SIMON_STATE_DIE || simon->GetState() == SIMON_STATE_AUTO_WALKING)
		return;

	if (game->IsKeyDown(DIK_RIGHT)) {
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (simon->isGround == true)
			simon->SetState(SIMON_STATE_SIT);
	}
	else {
		simon->SetState(SIMON_STATE_IDLE);
	}
}

void CPlayScene::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	if (simon->GetState() == SIMON_STATE_ITEM || simon->GetState() == SIMON_STATE_AUTO_WALKING) 
		return;

	if (simon->GetState() == SIMON_STATE_AUTO_WALKING && simon->GetEntraceTime() && GetTickCount() - simon->GetEntraceTime() > SIMON_ATTACK_TIME)
		return;

	if (simon->GetAttackTime())
		return;

	if (KeyCode == DIK_SPACE) {
		if (simon->isGround == false || simon->IsHitting() == true) return;
		simon->SetState(SIMON_STATE_JUMP);
	}
	else if (KeyCode == DIK_Z) {
		if (simon->GetAttackTime() == 0)
		{
			simon->isKnife = false;
			if (simon->GetState() == SIMON_STATE_SIT) simon->SetState(SIMON_STATE_SIT_ATTACK);
			else simon->SetState(SIMON_STATE_STAND_ATTACK);
		}
	}
	else if (KeyCode == DIK_C) {
		if (simon->GetAttackTime() == 0
			&& simon->subWeapon != SUBWEAPON::DEFAULT)
		{
			simon->spawnKnife = true;
			simon->isKnife = true;
			if (simon->GetState() == SIMON_STATE_SIT) simon->SetState(SIMON_STATE_SIT_ATTACK);
			else simon->SetState(SIMON_STATE_STAND_ATTACK);
		}
	}
}

