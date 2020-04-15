#pragma once
#include<Windows.h>

#include "Simon.h"
#include "Brick.h"
#include "Goomba.h"
#include "Whip.h"
#include "Candle.h"
#include "TileMap.h"
#include "Effect.h"
#include "IHeart.h"
#include "IWhip.h"
#include "IKnife.h"

class PlayScene
{
	CSimon* simon;
	CGoomba* goomba;
	CEffect* effect;
	CTileMap* tilemap;
	CCandle* candle;
	vector<LPGAMEOBJECT> objects;	
public:
	PlayScene(){}
	void LoadResource();
	void Update(DWORD dt);
	void Render();

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
};

