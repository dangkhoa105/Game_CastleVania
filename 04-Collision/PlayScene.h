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

class CPlayScene
{
	CSimon* simon;
	CGoomba* goomba;
	CEffect* effect;
	CTileMap* tilemap;
	CCandle* candle;
	vector<LPGAMEOBJECT> objects;	
	std::unordered_map<int, CPScene*> pScenes;

	CPScene* currentScene;
	CTileMaps gameMaps;

	

	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	ReadResourceFile* ReadResourceFile = ReadResourceFile::GetInstance();
	
public:
	CPlayScene(){
		
	}

	void LoadResource();
	void LoadMap();
	void LoadItem();
	
	void Update(DWORD dt);
	void UpdateCam();
	void UpdateItem();
	void UpdateScene();

	void Render();
	void Unload();

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
};

