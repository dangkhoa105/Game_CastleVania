#include "PlayScene.h"
#include <dinput.h>
#include "debug.h"
#include "Game.h"

CPlayScene* CPlayScene::__instance = NULL;

void CPlayScene::GetListobjectFromGrid()
{
	objects.clear();

	while (newObjectList.size() > 0)
	{
		grid->AddWhenUpdate(newObjectList.front());
		newObjectList.pop();
	}

	grid->GetListobjectFromGrid(objects);
}

void CPlayScene::UpdateGrid()
{
	for (size_t i = 0; i < this->objects.size(); i++)
	{
		LPGAMEOBJECT obj = this->objects.at(i);
		float x_, y_;
		obj->GetPosition(x_, y_);
		grid->Update(obj);
	}
}

CPlayScene* CPlayScene::GetInstance()
{
	if (__instance == NULL) __instance = new CPlayScene();
	return __instance;
}

void CPlayScene::LoadResource()
{
	CGame* game = CGame::GetInstance();


	LoadMap();
	LoadGrids();

	LPANIMATION ani;

	hud = new HUD();

	simon = new CSimon();
	simon->SetPosition(currentScene->simonX, currentScene->simonY);
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
				const int camRight = std::atoi(grand->first_attribute("camRight")->value());
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
				pScene->camRight = camRight;

				pScenes.insert(std::make_pair(id, pScene));
			}
		}
	}

	this->currentScene = pScenes.at(activeScene);

	this->tilemap = this->tileMaps.Get(this->currentScene->mapId);

}

void CPlayScene::LoadGrids()
{
	const std::string path = "resources\\CastleVania\\castlevania.xml";
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("grids");
	int objCollectId = 0;

	for (xml_node<>* child = rootNode->first_node(); child; child = child->next_sibling()) //cú pháp lập
	{
	
		const std::string childPath = std::string(child->first_attribute("path")->value());
		const int gridID = std::atoi(child->first_attribute("gridId")->value());
		Grid* grid = new Grid();
		grid->LoadGridObject(childPath);
		this->grids.insert(std::make_pair(gridID, grid));
	}
	this->grid = this->grids.at(this->currentScene->mapId);
}

void CPlayScene::AddtoGrid(LPGAMEOBJECT object, bool isAlwayUpdate)
{
	grid->Add(object, isAlwayUpdate);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Simon is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	if (simon->y > currentScene->camY + tilemap->GetMapHeight())
		simon->SetState(SIMON_STATE_DIE);

	if (simon->GetState() == SIMON_STATE_DIE)
	{
		if (resetSimonTime == 0)
			resetSimonTime = GetTickCount();
		else if (GetTickCount() - resetSimonTime > RESETSIMONTIME)
		{ 
			simon->ResetHPHeart();
			simon->idChangeScene = currentScene->mapId;
			simon->isColliWithTrigger = false;
			lockCam = false;
			UpdateScene();
			resetSimonTime = 0;
		}
	}

	GetListobjectFromGrid();
	DebugOut(L"Ga e objest: %d \n", this->objects.size());
	UpdateSubWeapon();

	vector<LPGAMEOBJECT> coObjects;
	coObjects.push_back(simon);
	for (int i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects.at(i));
	}
	simon->Update(dt, &coObjects);

	for (int i = 0; i < objects.size(); i++)
	{
		objects.at(i)->Update(dt, &coObjects);
	}

	CBrick* brick = new CBrick();
	if (simon->isColliWithTrigger)
	{
		brick->SetPosition(976, 224);
		brick->widthbbox = 32;
		brick->heightbbox = 92;
		grid->AddWhenUpdate(brick);
		simon->isColliWithTrigger = false;
		lockCam = true;
	}

	UpdateGameTime();

	UpdateItem();

	if (!lockCam)
		UpdateCam();	

	UpdateGrid();

	UpdatePositionSpawnEnemy();

	Unload();

	UpdateScene();

	hud->Update();
}

void CPlayScene::Render()
{
	CGame* game = CGame::GetInstance();

	tilemap->Draw(game->GetCamPos());

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects.at(i)->isDestroy)
		{
			return;
		}
		objects.at(i)->Render();
	}
	hud->Render();
	simon->Render();

	if (simon->GetState() == SIMON_STATE_AUTO_WALKING)
		wall->Render();
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
	cx -= SCREEN_WIDTH / 2 - 30;
	cy -= SCREEN_HEIGHT / 2;
	if (cx > currentScene->camX && cx < currentScene->camX + tilemap->GetMapWidth() - SCREEN_WIDTH)
	{
		CGame::GetInstance()->SetCamPos(cx, currentScene->camY);
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
				auto effect = new CEffect();
				effect->SetPosition(x, y);
				effect->SetState(EFFECT_STATE_CANDLE);
				grid->AddWhenUpdate(effect);

				if (f->GetItem() != -1)
				{
					auto item = new CItem();
					if (f->GetItem() == ID_IHEART)
					{
						item->SetId(ID_IHEART);
					}
					else if (f->GetItem() == ID_SMALL_IHEART)
					{
						item->SetId(ID_SMALL_IHEART);
						item->SetPositionInit(x);
					}
					else if (f->GetItem() == ID_IWHIP)
					{
						item->SetId(ID_IWHIP);
					}
					else if (f->GetItem() == ID_IKNIFE)
					{
						item->SetId(ID_IKNIFE);
					}
					else if (f->GetItem() == ID_IBOOMERANG)
					{
						item->SetId(ID_IBOOMERANG);
					}
					else if (f->GetItem() == ID_IAXE)
					{
						item->SetId(ID_IAXE);
					}
					else if (f->GetItem() == ID_ICLOCK)
					{
						item->SetId(ID_ICLOCK);
					}
					else if (f->GetItem() == ID_IGAS)
					{
						item->SetId(ID_IGAS);
					}
					else if (f->GetItem() == ID_IMONEYBAG_400)
					{
						item->SetId(ID_IMONEYBAG_400);
					}
					else if (f->GetItem() == ID_IMONEYBAG_700)
					{
						item->SetId(ID_IMONEYBAG_700);
					}
					else if (f->GetItem() == ID_IMONEYBAG_1000)
					{
						item->SetId(ID_IMONEYBAG_1000);
					}
					else if (f->GetItem() == ID_IPOTION)
					{
						item->SetId(ID_IPOTION);
					}
					else if (f->GetItem() == ID_IDOUBLESHOT)
					{
						item->SetId(ID_IDOUBLESHOT);
					}
					item->SetPosition(x, y);
					grid->AddWhenUpdate(item);
				}
				
			}
		}
		if (dynamic_cast<CBoss*>(objects.at(i)))
		{
			auto f = dynamic_cast<CBoss*>(objects.at(i));

			if (f->IsDestroy() && f->count_start == 0)
			{
				float x, y;
				f->GetPosition(x, y);
				auto effect = new CEffect();
				effect->SetPosition(x, y);
				effect->SetState(EFFECT_STATE_ENEMIES);
				grid->AddWhenUpdate(effect);
			}
		}
		if (dynamic_cast<CEnemy*>(objects.at(i)))
		{
			auto f = dynamic_cast<CEnemy*>(objects.at(i));

			if (f->IsDestroy() && f->count_start == 0)
			{
				float x, y;
				f->GetPosition(x, y);
				auto effect = new CEffect();
				effect->SetPosition(x, y);
				effect->SetState(EFFECT_STATE_ENEMIES);
				grid->AddWhenUpdate(effect);
			}
		}
		if (dynamic_cast<CMoneyBagTrigger*>(objects.at(i)))
		{
			auto f = dynamic_cast<CMoneyBagTrigger*>(objects.at(i));
			if (f->IsDestroy())
			{
				auto moneyBag = new CItem();
				moneyBag->SetId(ID_IMONEYBAG_SPECIAL);
				moneyBag->SetPosition(1207, 240);
				grid->AddWhenUpdate(moneyBag);
			}
		}
		if (dynamic_cast<CBreakWallTrigger*>(objects.at(i)))
		{
			auto f = dynamic_cast<CBreakWallTrigger*>(objects.at(i));
			if (f->IsDestroy())
			{
				auto crown = new CItem();
				crown->SetId(ID_ICROWN);
				crown->SetPosition(256, 270);
				grid->AddWhenUpdate(crown);
			}
		}
		if (dynamic_cast<CItem*>(objects.at(i)))
		{
			auto f = dynamic_cast<CItem*>(objects.at(i));

			if (f->IsDestroy() && (f->GetId() == ID_IMONEYBAG_SPECIAL || 
				f->GetId() == ID_IMONEYBAG_400 || 
				f->GetId() == ID_IMONEYBAG_700 || 
				f->GetId() == ID_IMONEYBAG_1000 ||
				f->GetId() == ID_ICROWN))
			{
				float x, y;
				f->GetPosition(x, y);	
				auto effect = new CEffect();
				effect->SetPosition(x + IMONEYBAG_BBOX_WIDTH, y);

				if (f->GetId() == ID_IMONEYBAG_SPECIAL)
				{
					effect->SetState(EFFECT_STATE_MONEYBAG_SPECIAL);
					f->SetId(ID_IMONEYBAG_SPECIAL);
				}
				else if (f->GetId() == ID_IMONEYBAG_400)
				{
					effect->SetState(EFFECT_STATE_MONEYBAG_400);
					f->SetId(ID_IMONEYBAG_400);
				}
				else if (f->GetId() == ID_IMONEYBAG_700)
				{
					effect->SetState(EFFECT_STATE_MONEYBAG_700);
					f->SetId(ID_IMONEYBAG_700);
				}
				else if (f->GetId() == ID_IMONEYBAG_1000)
				{
					effect->SetState(EFFECT_STATE_MONEYBAG_1000);
					f->SetId(ID_IMONEYBAG_1000);
				}
				else if (f->GetId() == ID_ICROWN)
				{
					effect->SetState(EFFECT_STATE_2000);
					f->SetId(ID_ICROWN);
				}				
				
				grid->AddWhenUpdate(effect);
			}
		}
		if (dynamic_cast<CBreakWall*>(objects.at(i)))
		{
			auto f = dynamic_cast<CBreakWall*>(objects.at(i));

			if (f->IsDestroy())
			{
				auto black = new CEffect();
				black->SetState(EFFECT_STATE_BREAKWALL_DESTROYED);
				black->SetPosition(f->x, f->y);
				grid->AddWhenUpdate(black);
				if (currentScene->mapId == 1)
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
						grid->AddWhenUpdate(derbir);
					}
				}
				else if (currentScene->mapId == 2 || currentScene->mapId == 6)
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
						grid->AddWhenUpdate(derbir);
					}

					auto doubleShot = new CItem();
					doubleShot->SetId(ID_IDOUBLESHOT);
					doubleShot->SetPosition(f->x, f->y);
					grid->AddWhenUpdate(doubleShot);
				}
				else if (currentScene->mapId == 4)
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
						grid->AddWhenUpdate(derbir);
					}

					auto kfc = new CItem();
					kfc->SetId(ID_IKFC);
					kfc->SetPosition(f->x, f->y);
					grid->AddWhenUpdate(kfc);
				}
			}
		}
		if (dynamic_cast<CBoss*>(objects.at(i)))
		{
			auto f = dynamic_cast<CBoss*>(objects.at(i));
			if (simon->whip->isColliWithBoss|| simon->subWeapon->isColliWithBoss)
			{
				auto black = new CEffect();
				black->SetState(EFFECT_STATE_ENEMIES);
				black->SetPosition(f->x, f->y);
				grid->AddWhenUpdate(black);
				simon->whip->isColliWithBoss = false;
				simon->subWeapon->isColliWithBoss = false;
			}
		}
	}
}

void CPlayScene::UpdateScene()
{
	if (simon->idChangeScene != -1)
	{
		SwitchScene(simon->idChangeScene);
		simon->isOnStair = false;
		simon->isColliWithStair = false;
		this->tilemap = this->tileMaps.Get(this->currentScene->mapId);
		this->grid = this->grids.at(this->currentScene->mapId);
		simon->SetNx(currentScene->nx);
		simon->SetState(currentScene->stateSimon);
		simon->SetPosition(currentScene->simonX, currentScene->simonY);
		simon->lastStepOnStairPos = { float(currentScene->simonX), float(currentScene->simonY) };

		//CGame::GetInstance()->SetCamPos(currentScene->camX - SCREEN_WIDTH, currentScene->camY);
		if (this->currentScene->camRight == 0)
			CGame::GetInstance()->SetCamPos(currentScene->camX, currentScene->camY);
		else
			CGame::GetInstance()->SetCamPos(currentScene->camX + tilemap->GetMapWidth() - SCREEN_WIDTH, currentScene->camY);
		simon->idChangeScene = -1;
	}
}

void CPlayScene::UpdatePositionSpawnEnemy()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CZombie*>(objects.at(i)))
		{
			auto f = dynamic_cast<CZombie*>(objects.at(i));
			if (f->GetState() != ZOMBIE_STATE_IDLE && f->isFinishReSpawn == false)
			{
				f->isFinishReSpawn = true;

				f->SetState(ZOMBIE_STATE_WALKING);
			}
		}
		if (dynamic_cast<CGhost*>(objects.at(i)))
		{
			auto f = dynamic_cast<CGhost*>(objects.at(i));
			if (f->GetState() != GHOST_STATE_IDLE && f->isFinishReSpawn == false)
			{
				f->isFinishReSpawn = true;

				f->SetState(GHOST_STATE_FLYING);
			}
		}
		if (dynamic_cast<CSkeleton*>(objects.at(i)))
		{
			auto f = dynamic_cast<CSkeleton*>(objects.at(i));
			if (f->GetState() != SKELETON_STATE_IDLE && f->isFinishReSpawn == false)
			{
				f->isFinishReSpawn = true;

				f->SetState(SKELETON_STATE_JUMPING);
			}
		}
	}

}

void CPlayScene::UpdateGameTime()
{
	if (gameCountTime == 0)
		gameCountTime = GetTickCount();

	if (gameCountTime != 0 && GetTickCount() - gameCountTime >= 1000)
	{
		if (gameTime > 0)
			gameTime--;

		gameCountTime = 0;
	}
}

void CPlayScene::UpdateSubWeapon()
{
	if (simon->spawnSubWeapon)
	{
		countSubWeapon++;

		auto subWeapon = new CSubWeapon();
		switch (simon->subWeapons)
		{
		case SUBWEAPON::KNIFE:
			subWeapon->nx = simon->nx;
			subWeapon->SetState(STATE_KNIFE);
			if (simon->GetState() == SIMON_STATE_SIT_ATTACK)
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_X * 2);
			else
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_Y);
			break;
		case SUBWEAPON::BOOMERANG:
			subWeapon->nx = simon->nx;
			subWeapon->SetState(STATE_BOOMERANG);
			if (simon->GetState() == SIMON_STATE_SIT_ATTACK)
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_X);
			else
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_Y);
			break;
		case SUBWEAPON::AXE:
			subWeapon->nx = simon->nx;
			subWeapon->SetState(STATE_AXE);
			if (simon->GetState() == SIMON_STATE_SIT_ATTACK)
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_X);
			else
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_Y);
			break;
		case SUBWEAPON::CLOCK:
			subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_X * 2);
			subWeapon->SetState(STATE_CLOCK);
			break;
		case SUBWEAPON::GAS:
			subWeapon->nx = simon->nx;
			subWeapon->SetState(STATE_GAS);
			if (simon->GetState() == SIMON_STATE_SIT_ATTACK)
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_X);
			else
				subWeapon->SetPosition(simon->x, simon->y + SUBWEAPON_POSITION_Y);
			break;
		}
		grid->AddWhenUpdate(subWeapon);
		simon->spawnSubWeapon = false;
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

	if (simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_DIE)
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
			if (simon->IsHitting())
			{
				float tempt = simon->vx;
				simon->SetState(SIMON_STATE_IDLE);
				simon->vx = tempt;
			}

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
				simon->isUpStair = true;
			}
			else if (simon->GetState() == SIMON_STATE_STAIR_UP_IDLE) {

				simon->SetStartOnStair();
				simon->isUpStair = true;
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
			simon->isUpStair = false;
			DebugOut(L"Simon down \n");
			return;
		}
		else if (simon->CheckCanStepDown()) {
			if (!simon->CheckIsOnStair() && simon->CheckColliWithStair()) {
				simon->SetStartOnStair();
				simon->isUpStair = false;
			}
			else if (simon->GetState() == SIMON_STATE_STAIR_DOWN_IDLE) {

				simon->SetStartOnStair();
				simon->isUpStair = false;
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
	CGame* game = CGame::GetInstance();

	if (simon->GetState() == SIMON_STATE_ITEM || simon->GetState() == SIMON_STATE_AUTO_WALKING || simon->GetState() == SIMON_STATE_HURT || simon->GetState() == SIMON_STATE_DIE)
		return;

	if (simon->GetAttackTime())
		return;

	switch (KeyCode)
	{
	case DIK_1:
		simon->idChangeScene = 0;
		break;
	case DIK_2:
		simon->idChangeScene = 1;
		break;
	case DIK_3:
		simon->idChangeScene = 2;
		break;
	case DIK_4:
		simon->idChangeScene = 4;
		break;
	case DIK_5:
		simon->idChangeScene = 5;
		break;
	case DIK_6:
		simon->idChangeScene = 6;
		break;
	default:
		break;
	}

	if (KeyCode == DIK_SPACE) {
		DebugOut(L"space \n");
		if (!simon->isGround ||
			!simon->IsHitting() == true &&
			!simon->CheckIsOnStair() &&
			!simon->CheckStartOnStair() &&
			simon->GetState() == SIMON_STATE_JUMP)
			return;
		simon->SetState(SIMON_STATE_JUMP);
	}
	else if (KeyCode == DIK_Z) {
		if (simon->GetAttackTime() == 0)
		{
			if (game->IsKeyDown(DIK_UP) && simon->subWeapons != SUBWEAPON::DEFAULT && simon->GetHeart() != 0)
			{
				if (simon->isDoubleShot && countSubWeapon < 2)
				{
					if (simon->subWeapons == SUBWEAPON::CLOCK)
					{
						countSubWeapon--;
					}
					simon->subWeapon->fight = true;
					simon->SetHeart();
					simon->spawnSubWeapon = true;
					simon->isSubWeapon = true;
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
						else 
							simon->SetState(SIMON_STATE_STAND_ATTACK);
					}
				}
				else if (!simon->isDoubleShot && countSubWeapon < 1)
				{
					if (simon->subWeapons == SUBWEAPON::CLOCK)
					{
						countSubWeapon--;
					}
					simon->subWeapon->fight = true;
					simon->SetHeart();
					simon->spawnSubWeapon = true;
					simon->isSubWeapon = true;
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
			else
			{
				simon->whip->fight = true;
				simon->isSubWeapon = false;
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
}

