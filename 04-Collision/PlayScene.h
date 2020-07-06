#pragma once
#include<Windows.h>
#include "Simon.h"
#include "PScene.h"
#include "Brick.h"
#include "Goomba.h"
#include "Whip.h"
#include "Candle.h"
#include "TileMap.h"
#include "Effect.h"
#include "Entrace.h"
#include "ChangeScene.h"
#include "MoneyBagTrigger.h"	
#include "Item.h"
#include "Wall.h"
#include "Stair.h"
#include "SpearGuard.h"
#include "Bat.h"
#include "Bridge.h"
#include "BreakWall.h"
#include "BreakWallTrigger.h"
#include "Ghost.h"
#include "Monkey.h"
#include "Skeleton.h"
#include "Crow.h"
#include "Zombie.h"
#include "Boss.h"
#include <queue>
#include "Grid.h"
#include "HUD.h"

class CPlayScene
{
	static CPlayScene* __instance;

	HUD* hud;
	CSimon* simon;
	CSkeleton* skeleton;
	CBat* bat;
	CGoomba* goomba;
	CWall* wall;
	CChangeScene* changeScene;
	CEffect* effect;

	DWORD gameTime = GAMETIME;
	DWORD gameCountTime = 0;

	DWORD resetSimonTime = 0;

	CTileMap* tilemap;
	CTileMaps tileMaps;

	std::queue<LPGAMEOBJECT> newObjectList;

	vector<LPGAMEOBJECT> objects;//ds object lay len tu grid moi lan update	
	std::unordered_map<int, CPScene*> pScenes;
	//std::unordered_map<int, vector<LPGAMEOBJECT>*> pMapObjects;

	CPScene* currentScene;

	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	ReadResourceFile* ReadResourceFile = ReadResourceFile::GetInstance();

	Grid* grid;// currentGrid;
	std::unordered_map<int, Grid*> grids;

	int countSubWeapon = 0;

	void GetListobjectFromGrid();
	void UpdateGrid();

public:

	CPlayScene() {};
	static CPlayScene* GetInstance();

	CSimon* GetSimon() { return this->simon; }

	void LoadResource();
	void LoadMap();
	void LoadObject();
	void AddNewObject(LPGAMEOBJECT obj) {
		newObjectList.push(obj);
	}


	void AddtoGrid(LPGAMEOBJECT object, bool isAlwayUpdate = false);
	void Update(DWORD dt);
	void UpdateCam();
	void UpdateItem();
	void UpdateScene();
	void UpdatePositionSpawnEnemy();
	void UpdateGameTime();
	void UpdateSubWeapon();

	void Render();
	void Unload();

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);

	void SwitchScene(int idScene)
	{
		this->currentScene = this->pScenes.at(idScene);
	}

	DWORD GetGameTime() { return this->gameTime; }

	int GetCurrentScene() {
		return this->currentScene->id;
	};

	int GetCountSW() { return countSubWeapon; }
	void SetCountSW() { countSubWeapon--; }
};
