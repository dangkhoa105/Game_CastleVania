#include "PlayScene.h"
#include <dinput.h>
#include "debug.h"
#include "Game.h"

void CPlayScene::LoadResource()
{
	CGame* game = CGame::GetInstance();

	LoadMap();
	LoadObject();

	LPANIMATION ani;

	this->objects = this->pMapObjects.at(this->currentScene->objCollectId);

	simon = new CSimon();
	simon->SetPosition(100, 200);
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
		// Lưu danh các thuộc tính của map
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

				tileMaps.Add(tilemap);
			}
		}
		// Lưu các thuộc tính của scene để chia scene và các object trong scene
		else if (nodeName == "scenes")
		{
			activeScene = std::atoi(child->first_attribute("activeID")->value());

			for (xml_node<>* grand = child->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				const int id = std::atoi(grand->first_attribute("id")->value());
				const int mapID = std::atoi(grand->first_attribute("mapId")->value());
				const int objCollectId = std::atoi(grand->first_attribute("objcollectid")->value());
				CPScene* pScene = new CPScene();
				pScene->id = id;
				pScene->mapId = mapID;
				pScene->objCollectId = objCollectId;

				pScenes.insert(std::make_pair(id, pScene));
			}
		}
	}

	this->currentScene = pScenes.at(activeScene);

	this->tilemap = this->tileMaps.Get(this->currentScene->mapId);
}

void CPlayScene::LoadObject()
{
	//load objects item
	const std::string path = "resources\\CastleVania\\castlevania.xml";
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("objectsCollection");
	int objCollectId = 0;
	for (xml_node<>* child = rootNode->first_node(); child; child = child->next_sibling()) //cú pháp lập
	{
		const std::string childPath = std::string(child->first_attribute("path")->value());

		const int objID = std::atoi(child->first_attribute("objcollectid")->value());
		rapidxml::file<> xmlFile(childPath.c_str());
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		xml_node<>* objectNode = doc.first_node("objects");
		std::vector<LPGAMEOBJECT>* _object = new std::vector<LPGAMEOBJECT>();
		for (xml_node<>* ochild = objectNode->first_node(); ochild; ochild = ochild->next_sibling()) //cú pháp lập
		{
			const std::string nodeName = ochild->name();
			if (nodeName == "bricks")
			{
				for (xml_node<>* grand = ochild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
				{
					CBrick* brick = new CBrick();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					const int widthbbox = std::atoi(grand->first_attribute("widthbbox")->value());
					const int heightbbox = std::atoi(grand->first_attribute("heightbbox")->value());
					brick->SetPosition(x, y);
					brick->widthbbox = widthbbox;
					brick->heightbbox = heightbbox;
					_object->push_back(brick);
				}
			}
			if (nodeName == "candles")
			{
				for (xml_node<>* grand = ochild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
				{
					CCandle* candle = new CCandle();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					const int itemID = std::atoi(grand->first_attribute("itemId")->value());
					candle->SetItem(itemID);
					candle->SetPosition(x, y);
					_object->push_back(candle);
				}
			}
			if (nodeName == "entrace")
			{
				CEntrace* entrace = new CEntrace();
				const int x = std::atoi(ochild->first_attribute("x")->value());
				const int y = std::atoi(ochild->first_attribute("y")->value());
				entrace->SetPosition(x, y);
				_object->push_back(entrace);
			}
			if (nodeName == "changeScene")
			{
				CChangeScene* changeScene = new CChangeScene();
				const int x = std::atoi(ochild->first_attribute("x")->value());
				const int y = std::atoi(ochild->first_attribute("y")->value());
				const int idChangeScene = std::atoi(ochild->first_attribute("idChangeScene")->value());
				changeScene->SetPosition(x, y);
				changeScene->SetIdNextScene(idChangeScene);
				_object->push_back(changeScene);
			}
			if (nodeName == "moneyBagTrigger")
			{
				CMoneyBagTrigger* moneyBagTrigger = new CMoneyBagTrigger();
				const int x = std::atoi(ochild->first_attribute("x")->value());
				const int y = std::atoi(ochild->first_attribute("y")->value());
				moneyBagTrigger->SetPosition(x, y);
				_object->push_back(moneyBagTrigger);
			}
			if (nodeName == "wall")
			{
				wall = new CWall();
				const int x = std::atoi(ochild->first_attribute("x")->value());
				const int y = std::atoi(ochild->first_attribute("y")->value());
				wall->SetPosition(x, y);
				_object->push_back(wall);
			}
			if (nodeName == "stairs")
			{
				for (xml_node<>* grand = ochild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
				{
					CStair* stair = new CStair();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					const int stairDirection = std::atoi(grand->first_attribute("stairDirection")->value());
					stair->SetPosition(x, y);
					stair->SetDirectionStair(stairDirection);
					_object->push_back(stair);
				}
			}
		}
		this->pMapObjects.insert(std::make_pair(objID, _object));
	}
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Simon is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	UpdateScene();

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
			this->objects->push_back(knife);
			break;
		}
		simon->spawnKnife = false;
	}

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 0; i < objects->size(); i++)
	{
		if (dynamic_cast<CSimon*>(objects->at(i)))
		{
			continue;
		}
		coObjects.push_back(objects->at(i));
	}
	simon->Update(dt, &coObjects);
	//simon->whip->Update(dt, objects);

	for (int i = 0; i < objects->size(); i++)
	{
		objects->at(i)->Update(dt, &coObjects);
	}

	UpdateItem();

	UpdateCam();

	Unload();	
}

void CPlayScene::Render()
{
	CGame* game = CGame::GetInstance();

	tilemap->Draw(game->GetCamPos());

	for (int i = 0; i < objects->size(); i++)
	{
		if (objects->at(i)->isDestroy)
		{
			return;
		}
		objects->at(i)->Render();
	}
	simon->Render();

	if (simon->GetState() == SIMON_STATE_AUTO_WALKING)
		wall->Render();
}

void CPlayScene::Unload()
{
	for (vector<LPGAMEOBJECT>::iterator it = objects->begin(); it != objects->end(); ) {

		if ((*it)->IsDestroy()) {
			it = objects->erase(it);
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
		/*CTileMap* tilemap = this->gameMaps.Get(this->currentScene->mapId);
		int min_col = tilemap->min_max_col_to_draw[tilemap->index][0];
		int max_col = tilemap->min_max_col_to_draw[tilemap->index][1];*/
		cx -= SCREEN_WIDTH / 2;
		//CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
		/*if (simon->x >= min_col * 32 + (SCREEN_WIDTH / 2 - 16) &&
			simon->x <= max_col * 32 - (SCREEN_WIDTH / 2 - 16))
		{*/
		CGame::GetInstance()->SetCamPos(cx, 0.0f);
		//}
	}
}

void CPlayScene::UpdateItem()
{
	for (int i = 0; i < objects->size(); i++)
	{
		if (dynamic_cast<CCandle*>(objects->at(i)))
		{
			auto f = dynamic_cast<CCandle*>(objects->at(i));

			if (f->IsDestroy())
			{
				float x, y;
				f->GetPosition(x, y);
				if (f->GetItem() != -1)
				{
					if (f->GetItem() == ID_IHEART)
					{
						auto heart = new CItem();
						heart->SetId(ID_IHEART);
						heart->SetPosition(x, y);
						objects->push_back(heart);
					}
					if (f->GetItem() == ID_IWHIP)
					{
						auto whip = new CItem();
						whip->SetId(ID_IWHIP);
						whip->SetPosition(x, y);
						objects->push_back(whip);
					}
					if (f->GetItem() == ID_IKNIFE)
					{
						auto knife = new CItem();
						knife->SetId(ID_IKNIFE);
						knife->SetPosition(x, y);
						objects->push_back(knife);
					}
				}
				auto effect = new CEffect();
				effect->SetPosition(x, y);
				objects->push_back(effect);
			}
		}
	}

	for (int i = 0; i < objects->size(); i++)
	{
		if (dynamic_cast<CMoneyBagTrigger*>(objects->at(i)))
		{
			auto f = dynamic_cast<CMoneyBagTrigger*>(objects->at(i));
			if (f->IsDestroy())
			{
				auto moneyBag = new CItem();
				moneyBag->SetId(ID_IMONEYBAG);
				moneyBag->SetPosition(1207, 240);
				objects->push_back(moneyBag);
			}
		}
	}
}

void CPlayScene::UpdateScene()
{
	if (simon->idChangeScene != -1)
	{
		SwitchScene(simon->idChangeScene);
		this->tilemap = this->tileMaps.Get(this->currentScene->mapId);
		//changeScene->SetIsChangeScene(false);
		simon->idChangeScene = -1;
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetPosition(50, 10);

		this->objects = this->pMapObjects.at(this->currentScene->objCollectId);
		CGame::GetInstance()->SetCamPos(0, 0);
	}
}

void CPlayScene::KeyState(BYTE* states)
{
	int counter = 0;
	CGame* game = CGame::GetInstance();
	if ((simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_IDLE) && simon->isGround == false)
		return;
	
	if (simon->CheckAutoWalk()) {
		return;
	}

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

	if (simon->GetState() == SIMON_STATE_ITEM || simon->GetState() == SIMON_STATE_DIE || simon->GetState() == SIMON_STATE_AUTO_WALKING)
		return;

	if (game->IsKeyDown(DIK_UP)) {
		if (simon->GetState() == SIMON_STATE_STAIR_DOWN_IDLE) {
			if (simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNLEFT)
				simon->SetOnStairDirection(STAIRDIRECTION::UPRIGHT);
			else if (simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNRIGHT)
				simon->SetOnStairDirection(STAIRDIRECTION::UPLEFT);
			simon->SetStartOnStair();
			DebugOut(L"Simon up to down \n");
			return;
		}
		else if (simon->CheckCanStepUp()) {
			if (!simon->CheckIsOnStair() && simon->CheckColliWithStair()) {
				simon->SetStartOnStair();
			}
			else if (simon->GetState() == SIMON_STATE_STAIR_UP_IDLE) {

				simon->SetStartOnStair();
			}
			return;
		}
	}

	if (simon->CheckIsOnStair() || simon->CheckStartOnStair())
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

	/*if (GetTickCount() - simon->GetEntraceTime() > SIMON_ATTACK_TIME)
		return;*/

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

