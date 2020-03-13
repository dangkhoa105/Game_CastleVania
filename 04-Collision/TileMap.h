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
	LPCWSTR filePath_tex;
	LPCWSTR filePath_data;
public:
	vector<vector<int>> min_max_col_to_draw;
	int index = 0;
	// ma tran du lieu map
	vector<vector<int>> map_Data;
	// luu cac tile
	//	CSprites * sprites;
	CTileMap(LPCWSTR filePath_data, int map_width, int map_height, int tile_width, int tile_height);
	//	TileMap(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height, int tile_width = 32, int tile_height = 32);
		// load tat ca cac block tile luu vao sprites
	void LoadResources();
	// load ma tran du lieu map
	/*void Load_MapData();*/
	// ve tat ca cac tile nam trong camera
	void Draw(D3DXVECTOR2 camPosition);
	int GetMapWidth() { return map_Width; }
	void SetTileMap(LPDIRECT3DTEXTURE9 texTM) { this->texMap = texTM; }
	//	void LoadMap();					// load ma trận map 
		//void CreateZoneToDraw();


	~CTileMap();
};
typedef CTileMap* LPTILEMAP;
