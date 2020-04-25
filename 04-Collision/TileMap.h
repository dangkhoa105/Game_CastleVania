#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Textures.h"
#include "Sprites.h"

class CTileMap
{
	CSprites* sprites = CSprites::GetInstance();

	vector<vector<LPSPRITE>> tilemap;
	LPDIRECT3DTEXTURE9 texMap;
	// chieu ngang cua map
	int map_Width;
	// chieu doc cua map
	int map_Height;
	// chi so hang 
	int nums_row;
	// chi so cot
	int nums_col;
	// chieu rong cua tileset
	int tile_Width;
	// chieu cao cua tileset
	int tile_Height;

	LPCWSTR filePath_data;

	int mapID;
public:
	int GetMapId() { return this->mapID; }
	vector<vector<int>> min_max_col_to_draw;
	int index = 0;

	vector<vector<int>> map_Data;

	CTileMap(LPCWSTR filePath_data, int map_width, int map_height, int tile_width, int tile_height,int mapID);

	void LoadResources();

	void Draw(D3DXVECTOR2 camPosition);
	int GetMapWidth() { return map_Width; }
	void SetTileMap(LPDIRECT3DTEXTURE9 texTM) { this->texMap = texTM; }

	~CTileMap();
};
typedef CTileMap* LPTILEMAP;

class CTileMaps 
{
	static CTileMaps* _instance;
	unordered_map<int, LPTILEMAP> tilemaps;
	
public:
	void Add(CTileMap* tileMap);
	LPTILEMAP Get(int ID) { return tilemaps[ID]; }

	static CTileMaps * GetInstance();
};

