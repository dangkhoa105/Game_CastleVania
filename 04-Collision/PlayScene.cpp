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
	simon->SetState(currentScene->stateSimon);
	simon->SetPosition(currentScene->simonX, currentScene->simonY);
	simon->lastStepOnStairPos = { float(currentScene->simonX), float(currentScene->simonY) };
	simon->SetNx(currentScene->nx);
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
				const int simonX = std::atoi(grand->first_attribute("simonX")->value());
				const int simonY = std::atoi(grand->first_attribute("simonY")->value());
				const int camX = std::atoi(grand->first_attribute("camX")->value());
				const int camY = std::atoi(grand->first_attribute("camY")->value());
				const int stateSimon = std::atoi(grand->first_attribute("state")->value());
				const int nx = std::atoi(grand->first_attribute("nx")->value());
				CPScene* pScene = new CPScene();
				pScene->id = id;
				pScene->mapId = mapID;
				pScene->objCollectId = objCollectId;
				pScene->simonX = simonX;
				pScene->simonY = simonY;
				pScene->camX = camY;
				pScene->camY = camY;
				pScene->stateSimon = stateSimon;
				pScene->nx = nx;

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
		for (xml_node<>* oChild = objectNode->first_node(); oChild; oChild = oChild->next_sibling()) //cú pháp lập
		{
			const std::string nodeName = oChild->name();
			if (nodeName == "bricks")
			{
				for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
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
			else if (nodeName == "candles")
			{
				for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
				{
					CCandle* candle = new CCandle();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					const int itemID = std::atoi(grand->first_attribute("itemId")->value());
					const int state = std::atoi(grand->first_attribute("state")->value());
					candle->SetItem(itemID);
					candle->SetPosition(x, y);
					candle->SetState(state);
					_object->push_back(candle);
				}
			}
			else if (nodeName == "entrace")
			{
				CEntrace* entrace = new CEntrace();
				const int x = std::atoi(oChild->first_attribute("x")->value());
				const int y = std::atoi(oChild->first_attribute("y")->value());
				entrace->SetPosition(x, y);
				_object->push_back(entrace);
			}
			else if (nodeName == "changeScene")
			{
				CChangeScene* changeScene = new CChangeScene();
				const int x = std::atoi(oChild->first_attribute("x")->value());
				const int y = std::atoi(oChild->first_attribute("y")->value());
				const int idChangeScene = std::atoi(oChild->first_attribute("idChangeScene")->value());
				changeScene->SetPosition(x, y);
				changeScene->SetIdNextScene(idChangeScene);
				_object->push_back(changeScene);
			}
			else if (nodeName == "moneyBagTrigger")
			{
				CMoneyBagTrigger* moneyBagTrigger = new CMoneyBagTrigger();
				const int x = std::atoi(oChild->first_attribute("x")->value());
				const int y = std::atoi(oChild->first_attribute("y")->value());
				moneyBagTrigger->SetPosition(x, y);
				_object->push_back(moneyBagTrigger);
			}
			else if (nodeName == "breakWallTrigger")
			{
				CBreakWallTrigger* breakWallTrigger = new CBreakWallTrigger();
				const int x = std::atoi(oChild->first_attribute("x")->value());
				const int y = std::atoi(oChild->first_attribute("y")->value());
				breakWallTrigger->SetPosition(x, y);
				_object->push_back(breakWallTrigger);
			}
			else if (nodeName == "wall")
			{
				wall = new CWall();
				const int x = std::atoi(oChild->first_attribute("x")->value());
				const int y = std::atoi(oChild->first_attribute("y")->value());
				wall->SetPosition(x, y);
				_object->push_back(wall);
			}
			else if (nodeName == "stairs")
			{
				for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
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
			else if (nodeName == "bridge")
			{
				CBridge* bridge = new CBridge();
				const int x = std::atoi(oChild->first_attribute("x")->value());
				const int y = std::atoi(oChild->first_attribute("y")->value());
				const int beginPositionX = std::atoi(oChild->first_attribute("beginPositionX")->value());
				const int lastPositionX = std::atoi(oChild->first_attribute("lastPositionX")->value());
				bridge->SetPosition(x, y);
				bridge->SetReturnPosition(beginPositionX, lastPositionX);
				_object->push_back(bridge);
			}
			else if (nodeName == "breakWalls")
			{
				for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
				{
					CBreakWall* breakWall = new CBreakWall();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					breakWall->SetPosition(x, y);
					_object->push_back(breakWall);
				}
			}
			else if (nodeName == "enemies")
			{
				for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
				{
					const std::string nodeNameGrand = grand->name();
					if (nodeNameGrand == "spearGuards")
					{
						for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
						{
							CSpearGuard* spearGuard = new CSpearGuard();
							const int x = std::atoi(oGrand->first_attribute("x")->value());
							const int y = std::atoi(oGrand->first_attribute("y")->value());
							const int beginPositionX = std::atoi(oGrand->first_attribute("beginPositionX")->value());
							const int lastPositionX = std::atoi(oGrand->first_attribute("lastPositionX")->value());
							const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
							const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
							const int bboxEnemyActiveWidth = std::atoi(oGrand->first_attribute("bboxEnemyActiveWidth")->value());
							const int bboxEnemyActiveHeight = std::atoi(oGrand->first_attribute("bboxEnemyActiveHeight")->value());
							spearGuard->SetPosition(x, y);
							spearGuard->SetReturnPosition(beginPositionX, lastPositionX);
							spearGuard->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
							spearGuard->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
							_object->push_back(spearGuard);
						}
					}
					else if (nodeNameGrand == "bat")
					{
						CBat* bat = new CBat();
						const int x = std::atoi(grand->first_attribute("x")->value());
						const int y = std::atoi(grand->first_attribute("y")->value());
						const int bboxEnemyWidth = std::atoi(grand->first_attribute("bboxEnemyWidth")->value());
						const int bboxEnemyHeight = std::atoi(grand->first_attribute("bboxEnemyHeight")->value());
						const int bboxEnemyActiveWidth = std::atoi(grand->first_attribute("bboxEnemyActiveWidth")->value());
						const int bboxEnemyActiveHeight = std::atoi(grand->first_attribute("bboxEnemyActiveHeight")->value());
						bat->SetPosition(x, y);
						bat->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
						bat->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
						_object->push_back(bat);
					}
					else if (nodeNameGrand == "ghost")
					{
						CGhost* ghost = new CGhost();
						const int x = std::atoi(grand->first_attribute("x")->value());
						const int y = std::atoi(grand->first_attribute("y")->value());
						const int beginPositionX = std::atoi(grand->first_attribute("beginPositionX")->value());
						const int lastPositionX = std::atoi(grand->first_attribute("lastPositionX")->value());
						const int bboxEnemyWidth = std::atoi(grand->first_attribute("bboxEnemyWidth")->value());
						const int bboxEnemyHeight = std::atoi(grand->first_attribute("bboxEnemyHeight")->value());
						const int bboxEnemyActiveWidth = std::atoi(grand->first_attribute("bboxEnemyActiveWidth")->value());
						const int bboxEnemyActiveHeight = std::atoi(grand->first_attribute("bboxEnemyActiveHeight")->value());
						ghost->SetPosition(x, y);
						ghost->SetReturnPosition(beginPositionX, lastPositionX);
						ghost->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
						ghost->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
						_object->push_back(ghost);
					}
					else if (nodeNameGrand == "monkeys")
					{
						for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
						{
							CMonkey* monkey = new CMonkey();
							const int x = std::atoi(oGrand->first_attribute("x")->value());
							const int y = std::atoi(oGrand->first_attribute("y")->value());
							const int beginPositionX = std::atoi(oGrand->first_attribute("beginPositionX")->value());
							const int lastPositionX = std::atoi(oGrand->first_attribute("lastPositionX")->value());
							const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
							const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
							const int bboxEnemyActiveWidth = std::atoi(oGrand->first_attribute("bboxEnemyActiveWidth")->value());
							const int bboxEnemyActiveHeight = std::atoi(oGrand->first_attribute("bboxEnemyActiveHeight")->value());
							monkey->SetPosition(x, y);
							monkey->SetReturnPosition(beginPositionX, lastPositionX);
							monkey->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
							monkey->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
							_object->push_back(monkey);
						}
					}
					else if (nodeNameGrand == "skeletons")
					{
						for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
						{
							CSkeleton* skeleton = new CSkeleton();
							const int x = std::atoi(oGrand->first_attribute("x")->value());
							const int y = std::atoi(oGrand->first_attribute("y")->value());
							const int beginPositionX = std::atoi(oGrand->first_attribute("beginPositionX")->value());
							const int lastPositionX = std::atoi(oGrand->first_attribute("lastPositionX")->value());
							const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
							const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
							const int bboxEnemyActiveWidth = std::atoi(oGrand->first_attribute("bboxEnemyActiveWidth")->value());
							const int bboxEnemyActiveHeight = std::atoi(oGrand->first_attribute("bboxEnemyActiveHeight")->value());
							skeleton->SetPosition(x, y);
							skeleton->SetReturnPosition(beginPositionX, lastPositionX);
							skeleton->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
							skeleton->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
							_object->push_back(skeleton);
						}
					}
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
	for (int i = 0; i < objects->size(); i++)
	{
		if (dynamic_cast<CSkeleton*>(objects->at(i)))
		{

			auto bone = new CBone();
			bone->nx = objects->at(i)->nx;
			bone->SetPosition(objects->at(i)->x, objects->at(i)->y + 30);
			this->objects->push_back(bone);
		}
	}

	vector<LPGAMEOBJECT> coObjects;

	for (int i = 0; i < objects->size(); i++)
	{
	/*	if (dynamic_cast<CSimon*>(objects->at(i)))
		{
			continue;
		}*/
		coObjects.push_back(simon);
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

	UpdateScene();

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
	cx -= SCREEN_WIDTH / 2 - 30;
	cy -= SCREEN_HEIGHT / 2;
	if (cx > currentScene->camX && cx < currentScene->camX + tilemap->GetMapWidth() - SCREEN_WIDTH)
	{
		//cx -= SCREEN_WIDTH / 2 - 30;
		//CGame::GetInstance()->SetCamPos(cx, 0.0f);
		CGame::GetInstance()->SetCamPos(cx, currentScene->camY);
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
						//heart->SetState(STATE_IHEART);
						objects->push_back(heart);
					}
					if (f->GetItem() == ID_SMALL_IHEART)
					{
						auto small_heart = new CItem();
						small_heart->SetId(ID_SMALL_IHEART);
						small_heart->SetPosition(x, y);
						//small_heart->SetState(STATE_SMALL_IHEART);
						objects->push_back(small_heart);
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
					if (f->GetItem() == ID_IBOOMERANG)
					{
						auto boomerang = new CItem();
						boomerang->SetId(ID_IBOOMERANG);
						boomerang->SetPosition(x, y);
						objects->push_back(boomerang);
					}
					if (f->GetItem() == ID_IMONEYBAG)
					{
						auto moneybag = new CItem();
						moneybag->SetId(ID_IMONEYBAG);
						moneybag->SetPosition(x, y);
						objects->push_back(moneybag);
					}
				}
				auto effect = new CEffect();
				effect->SetPosition(x, y);
				effect->SetState(EFFECT_STATE_CANDLE);
				objects->push_back(effect);
			}
		}
		if (dynamic_cast<CEnemy*>(objects->at(i)))
		{
			auto f = dynamic_cast<CEnemy*>(objects->at(i));

			if (f->IsDestroy())
			{
				float x, y;
				f->GetPosition(x, y);
				auto effect = new CEffect();
				effect->SetPosition(x, y);
				effect->SetState(EFFECT_STATE_CANDLE);
				objects->push_back(effect);
			}
		}
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
		if (dynamic_cast<CBreakWallTrigger*>(objects->at(i)))
		{
			auto f = dynamic_cast<CBreakWallTrigger*>(objects->at(i));
			if (f->IsDestroy())
			{
				auto crown = new CItem();
				crown->SetId(ID_ICROWN);
				crown->SetPosition(256, 270);
				objects->push_back(crown);
			}
		}
		if (dynamic_cast<CBreakWall*>(objects->at(i)))
		{
			auto f = dynamic_cast<CBreakWall*>(objects->at(i));

			if (f->IsDestroy())
			{
				if (currentScene->mapId == 1)
				{
					auto black = new CEffect();
					black->SetState(EFFECT_STATE_BREAKWALL_DESTROYED);
					black->SetPosition(f->x, f->y);
					objects->push_back(black);

					for (size_t i = 0; i < 4; i++)
					{
						auto derbir = new CEffect();
						derbir->SetState(EFFECT_STATE_BREAKWALL);
						derbir->SetPosition(f->x, f->y);
						float vx = (float)(-100 + rand() % 200) / 1000;
						float vy = (float)(-100 + rand() % 200) / 1000;
						derbir->vx = vx;
						derbir->vy = vy;
						objects->push_back(derbir);
					}
				}
				else if (currentScene->mapId == 2)
				{
					for (size_t i = 0; i < 4; i++)
					{
						auto derbir = new CEffect();
						derbir->SetState(EFFECT_STATE_BREAKWALL);
						derbir->SetPosition(f->x, f->y);
						float vx = (float)(-100 + rand() % 200) / 1000;
						float vy = (float)(-100 + rand() % 200) / 1000;
						derbir->vx = vx;
						derbir->vy = vy;
						objects->push_back(derbir);
					}

					auto doubleShot = new CItem();
					doubleShot->SetId(ID_IDOUBLESHOT);
					doubleShot->SetPosition(f->x, f->y);
					objects->push_back(doubleShot);
				}
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
		simon->SetState(currentScene->stateSimon);
		simon->SetPosition(currentScene->simonX, currentScene->simonY);		
		simon->lastStepOnStairPos = { float(currentScene->simonX), float(currentScene->simonY) };	
		simon->SetNx(currentScene->nx);
		//CGame::GetInstance()->SetCamPos(currentScene->camX - SCREEN_WIDTH, currentScene->camY);
		if (currentScene->nx == 1)
		CGame::GetInstance()->SetCamPos(currentScene->camX, currentScene->camY);
		else 
			CGame::GetInstance()->SetCamPos(currentScene->camX + tilemap->GetMapWidth() - SCREEN_WIDTH, currentScene->camY);
		simon->idChangeScene = -1;

		this->objects = this->pMapObjects.at(this->currentScene->objCollectId);
	}
}

void CPlayScene::KeyState(BYTE* states)
{
	int counter = 0;
	CGame* game = CGame::GetInstance();
	if ((simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_HURT) && simon->isGround == false)
		return;

	if (simon->CheckAutoWalk())
		return;

	if (simon->GetState() == SIMON_STATE_JUMP)
		return;


	//simon->GetUpgradeTime() tương đương  simon->GetUpgradeTime()!=0
	if (simon->GetUpgradeTime() && GetTickCount() - simon->GetUpgradeTime() > SIMON_UPGRADE_TIME)
	{
		simon->SetState(SIMON_STATE_IDLE);
		simon->ResetUpgradeTime();
	}

	if (simon->GetAttackTime() && GetTickCount() - simon->GetAttackTime() > SIMON_ATTACK_TIME)
	{
	
		if (simon->CheckIsOnStair()) {
			if (simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPLEFT || simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPRIGHT) {
				simon->SetState(SIMON_STATE_ATTACK_STAIR_UP);
				simon->SetState(SIMON_STATE_STAIR_UP_IDLE);
			}

			else if (simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNLEFT || simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNRIGHT) {
				simon->SetState(SIMON_STATE_ATTACK_STAIR_DOWN);
				simon->SetState(SIMON_STATE_STAIR_DOWN_IDLE);
			}

		}
		else
			simon->SetState(SIMON_STATE_IDLE);

		simon->ResetAttack();
		return;
	}

	if (simon->GetAttackTime() || simon->GetUpgradeTime())
		return;

	if (simon->GetState() == SIMON_STATE_ITEM || simon->GetState() == SIMON_STATE_AUTO_WALKING)
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
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (simon->GetState() == SIMON_STATE_STAIR_UP_IDLE) {
			if (simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPRIGHT)
			{


				simon->SetOnStairDirection(STAIRDIRECTION::DOWNLEFT);
			}
			else if (simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPLEFT)
				simon->SetOnStairDirection(STAIRDIRECTION::DOWNRIGHT);
			simon->SetStartOnStair();
			DebugOut(L"Simon down \n");
			return;
		}
		else if (simon->CheckCanStepDown()) {
			if (!simon->CheckIsOnStair() && simon->CheckColliWithStair()) {
				simon->SetStartOnStair();
			}
			else if (simon->GetState() == SIMON_STATE_STAIR_DOWN_IDLE) {

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
		{
			simon->SetState(SIMON_STATE_SIT);
			DebugOut(L"sitting \n");
		}
			
	}
	else {
		simon->SetState(SIMON_STATE_IDLE);
	}
}

void CPlayScene::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	if (simon->GetState() == SIMON_STATE_ITEM || simon->GetState() == SIMON_STATE_AUTO_WALKING || simon->GetState() == SIMON_STATE_HURT)
		return;

	if (simon->GetAttackTime())
		return;

	if (KeyCode == DIK_SPACE) {
		DebugOut(L"space \n");
		if (!simon->isGround||
			!simon->IsHitting() == true &&
			!simon->CheckIsOnStair() &&
			!simon->CheckStartOnStair() && 
			simon->GetState()== SIMON_STATE_JUMP)
			return;
		simon->SetState(SIMON_STATE_JUMP);
		DebugOut(L"2 \n");
	}
	else if (KeyCode == DIK_Z) {		
		if (simon->GetAttackTime() == 0)
		{			
			simon->whip->fight = true;
			simon->isKnife = false;
			if (simon->CheckIsOnStair()) {
				if (simon->GetState() == SIMON_STATE_STAIR_UP_IDLE &&
					(simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPLEFT || simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPRIGHT))
					simon->SetState(SIMON_STATE_ATTACK_STAIR_UP);
				else if (simon->GetState() == SIMON_STATE_STAIR_DOWN_IDLE &&
					(simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNLEFT || simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNRIGHT))
					simon->SetState(SIMON_STATE_ATTACK_STAIR_DOWN);
			}
			else {
				if (simon->GetState() == SIMON_STATE_SIT) simon->SetState(SIMON_STATE_SIT_ATTACK);
				else simon->SetState(SIMON_STATE_STAND_ATTACK);
			}
			DebugOut(L"3 \n");
		}
	}
	else if (KeyCode == DIK_C) {
		if (simon->GetAttackTime() == 0
			&& simon->subWeapon != SUBWEAPON::DEFAULT)
		{
			simon->spawnKnife = true;
			simon->isKnife = true;
			if (simon->CheckIsOnStair()) {
				if (simon->GetState() == SIMON_STATE_STAIR_UP_IDLE &&
					(simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPLEFT || simon->CheckStepOnStairDirection() == STAIRDIRECTION::UPRIGHT))
					simon->SetState(SIMON_STATE_ATTACK_STAIR_UP);
				else if (simon->GetState() == SIMON_STATE_STAIR_DOWN_IDLE &&
					(simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNLEFT || simon->CheckStepOnStairDirection() == STAIRDIRECTION::DOWNRIGHT))
					simon->SetState(SIMON_STATE_ATTACK_STAIR_DOWN);
			}
			else {
				if (simon->GetState() == SIMON_STATE_SIT) simon->SetState(SIMON_STATE_SIT_ATTACK);
				else simon->SetState(SIMON_STATE_STAND_ATTACK);
			}
		}
	}
}

