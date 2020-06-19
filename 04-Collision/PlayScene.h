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
#include <queue>

class CPlayScene
{
	static CPlayScene* __instance;



	CSimon* simon;
	CSkeleton* skeleton;
	CBat* bat;
	CGoomba* goomba;
	CWall* wall;
	CChangeScene* changeScene;
	CEffect* effect;
	CTileMap* tilemap;
	CTileMaps tileMaps;

	std::queue<LPGAMEOBJECT> newObjectList;

	vector<LPGAMEOBJECT>* objects;	
	
	std::unordered_map<int, CPScene*> pScenes;
	std::unordered_map<int, vector<LPGAMEOBJECT>*> pMapObjects;

	CPScene* currentScene;	

	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	ReadResourceFile* ReadResourceFile = ReadResourceFile::GetInstance();
	
public:
	CPlayScene() {};
	static CPlayScene* GetInstance();
	void LoadResource();
	void LoadMap();
	void LoadObject();
	void AddNewObject(LPGAMEOBJECT obj) {
		newObjectList.push(obj);
	}
	
	void Update(DWORD dt);
	void UpdateCam();
	void UpdateItem();
	void UpdateScene();

	void Render();
	void Unload();

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);

	void SwitchScene(int idScene)
	{
		this->currentScene = this->pScenes.at(idScene);
	}
};
