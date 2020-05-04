#include "TileMap.h"
#include "define.h"

CTileMap::CTileMap(LPCWSTR filePath_data, int map_width, int map_height, int tile_width, int tile_height,int id)
{
	this->filePath_data = filePath_data;

	this->sprites = CSprites::GetInstance();

	this->map_Width = map_width;
	this->map_Height = map_height;

	this->tile_Width = tile_width;
	this->tile_Height = tile_height;

	nums_row = map_Height / tile_Height;
	nums_col = map_Width / tile_Width;
	this->mapID = id;
}

void CTileMap::LoadResources()
{
	// lấy thông tin về kích thước của texture lưu các block tiles (filePath_tex)
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	this->texMap->GetLevelDesc(level, &surfaceDesc);

	// tính toán số hàng, số cột cần thiết để load tile 
	int nums_rowToRead = surfaceDesc.Height / tile_Height;
	int nums_colToRead = surfaceDesc.Width / tile_Width;

	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	int id_sprite = 1; 
	string a = "tilemap" + std::to_string(mapID) + std::to_string(id_sprite);
	
	for (int i = 0; i < nums_rowToRead; i++)
	{
		for (int j = 0; j < nums_colToRead; j++)
		{
			sprites->Add(a, tile_Width * j, tile_Height * i, tile_Width * (j + 1), tile_Height * (i + 1), this->texMap);
			id_sprite += 1;
			a = "tilemap" + std::to_string(mapID) + std::to_string(id_sprite);
		}
	}

	fstream fs;
	fs.open(filePath_data, ios::in);

	if (fs.fail())
	{
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);

		// tách số từ chuỗi đọc được
		// vector chua so trong 1 dong
		vector<int> numInLine;
		stringstream ss(line);
		int n;
		// doc tren mot dong 
		while (ss >> n)
		{
			// luu cac so vao dong
			numInLine.push_back(n);
		}
		// luu dng vao trong mapdata 
		map_Data.push_back(numInLine);
	}

	fs.close();
}

void CTileMap::Draw(D3DXVECTOR2 camPosition)
{
	int start_col_to_draw = camPosition.x / 32;
	
	int end_col_to_draw = ((camPosition.x + SCREEN_WIDTH) / 32);
	if (end_col_to_draw>=this->nums_col)
	{
		end_col_to_draw= this->nums_col-1;
	}
	for (int i = 0; i < nums_row; i++)
	{
		for (int j = start_col_to_draw; j <= end_col_to_draw; j++)
		{
			// camx để luôn giữ camera ở chính giữa, vì trong hàm draw có trừ cho camPosition.x làm các object đều di chuyển theo
			float x = tile_Width * (j - start_col_to_draw) + camPosition.x - (int)camPosition.x % 32; // vx*dt
			float y = tile_Height * i;

			string a = "tilemap" + std::to_string(mapID) + std::to_string(map_Data[i][j]);
			sprites->Get(a)->Draw(x, y);
		}
	}
}

CTileMap::~CTileMap()
{
}

CTileMaps* CTileMaps::_instance = NULL;

void CTileMaps::Add(CTileMap* tilemap)
{
	this->tilemaps.insert(std::make_pair(tilemap->GetMapId(), tilemap));
}

CTileMaps* CTileMaps::GetInstance()
{
	if (_instance == NULL) _instance = new CTileMaps();
	return _instance;
}
