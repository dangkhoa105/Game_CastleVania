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
#include "IHeart.h"
#include "IWhip.h"
#include "IKnife.h"
#include "Entrace.h"
#include "ChangeScene.h"
#include "MoneyBagTrigger.h"
#include "IMoneyBag.h"
#include "Item.h"
#include "Wall.h"

class CPlayScene
{
	CSimon* simon;
	CGoomba* goomba;
	CWall* wall;
	CChangeScene* changeScene;
	CEffect* effect;
	CTileMap* tilemap;
	CTileMaps tileMaps;

	vector<LPGAMEOBJECT>* objects;	

	std::unordered_map<int, CPScene*> pScenes;
	std::unordered_map<int, vector<LPGAMEOBJECT>*> pMapObjects;

	CPScene* currentScene;	
	bool switchScene = false;

	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	ReadResourceFile* ReadResourceFile = ReadResourceFile::GetInstance();
	
public:
	CPlayScene() {}

	void LoadResource();
	void LoadMap();
	void LoadObject();
	
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
		this->switchScene = true;
	}
};

