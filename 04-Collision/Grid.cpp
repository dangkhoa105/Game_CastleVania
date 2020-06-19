#include "Grid.h"
#include"debug.h"
#include"Game.h"
#include"Item.h"
#include"Effect.h"
#include"Enemy.h"
#include "Candle.h"

Grid::Grid(int mapWidth, int mapHeight) :
	mapWidth(mapWidth),
	mapHeight(mapHeight + 80)
{
	this->cellSize = CELL_SIZE;

	this->gridCol = ceil((float)this->mapWidth / this->cellSize) + 1;
	this->gridRow = ceil((float)this->mapHeight / this->cellSize);

	// clear grid
	this->grid.resize(gridRow);

	for (int i = 0; i < gridRow; i++)
		grid[i].resize(gridCol);

	for (size_t i = 0; i < gridRow; i++)
	{
		for (size_t j = 0; j < gridCol; j++)
		{
			this->grid[i][j].clear();
		}
	}
}

void Grid::Add(LPGAMEOBJECT object, bool isAlwayUpdateObject)
{
	float x_, y_;
	object->GetPosition(x_, y_);
	//tính vị trí của object
	int cellX = (int)(x_ / this->cellSize);
	int cellY = (int)(y_ / this->cellSize);
	if ( cellX >= gridCol || cellX < 0 ||cellY < 0 || cellY > gridRow - 1)
		return;

	//đánh dấu object thuộc cell nào
	// để có thể truy vấn nhanh vị trị của object trong grid

	object->SetCellIndex(cellX, cellY);
	if (!isAlwayUpdateObject)
		this->grid[cellY][cellX].push_back(object);
	else
		alwaysUpdateList.push_back(object);
}

void Grid::Update(LPGAMEOBJECT object)
{
	float cx_, cy_;

	D3DXVECTOR2 cam = CGame::GetInstance()->GetCamPos();

	cx_ = cam.x;
	cy_ = cam.y;

	float x, y;
	object->GetPosition(x, y);

	float l, t, r, b;
	object->GetBoundingBox(l, t, r, b);

	if (CGame::GetInstance()->AABB(l, t, r, b, cx_, cy_, cx_ + SCREEN_WIDTH, cy_ + SCREEN_HEIGHT))
		object->SetActive();
	else {
		if (object->CheckActive()) {
			if (dynamic_cast<CItem*>(object)
				|| dynamic_cast<CEnemy*>(object)
				) {
					object->isDestroy = true;
			}

		}
	}

	CellIndex oldCell = object->GetCellIndex();

	int cellX = (int)(x / this->cellSize);
	int cellY = (int)(y / this->cellSize);

	if (object->IsDestroy())
	{
		for (vector<LPGAMEOBJECT>::iterator it = grid[oldCell.y][oldCell.x].begin(); it != grid[oldCell.y][oldCell.x].end(); ) {
			if ((*it) == object) {
				it = grid[oldCell.y][oldCell.x].erase(it);
			}
			else ++it;
		}
		return;
	}

	if (oldCell.x == cellX && oldCell.y == cellY) return;
	if (oldCell.x != -1 && oldCell.y != -1) {				// loại bỏ cell cũ
		for (vector<LPGAMEOBJECT>::iterator it = grid[oldCell.y][oldCell.x].begin(); it != grid[oldCell.y][oldCell.x].end(); ) {
			if ((*it) == object) {
				it = grid[oldCell.y][oldCell.x].erase(it);
			}
			else ++it;
		}
	}
	Add(object);
}

void Grid::GetListobjectFromGrid(vector<LPGAMEOBJECT>& listobjects)
{
	vector<LPGAMEOBJECT> enemiesobject;
	vector<LPGAMEOBJECT> itemobject;
	vector<LPGAMEOBJECT> effectobject;
	enemiesobject.clear();
	itemobject.clear();
	effectobject.clear();

	float camx, camy;
	D3DXVECTOR2 cam = CGame::GetInstance()->GetCamPos();
	camx = cam.x;
	camy = cam.y;
	int startCol = floor(camx / this->cellSize);
	startCol = startCol > 0 ? startCol + -1 : 0;
	int endCol = floor((camx + SCREEN_WIDTH) / this->cellSize);
	endCol = endCol > gridCol ? gridCol : endCol + 1;
	for (int i = 0; i < this->gridRow; i++) {
		for (int j = startCol; j < endCol; j++) {
			for (size_t k = 0; k < this->grid[i][j].size(); k++) {
				LPGAMEOBJECT obj = this->grid[i][j].at(k);
				if (!obj->IsDestroy()) {
					if (dynamic_cast<CEnemy*>(obj))
						enemiesobject.push_back(obj);
					else if (dynamic_cast<CItem*>(obj))
						itemobject.push_back(obj);
					else if (dynamic_cast<CEffect*>(obj))
						effectobject.push_back(obj);
					else
						listobjects.push_back(obj);
				}
				else
				{
				}
			}
		}
	}

	listobjects.insert(listobjects.end(), alwaysUpdateList.begin(), alwaysUpdateList.end());
	listobjects.insert(listobjects.end(), itemobject.begin(), itemobject.end());
	listobjects.insert(listobjects.end(), enemiesobject.begin(), enemiesobject.end());
	listobjects.insert(listobjects.end(), effectobject.begin(), effectobject.end());
}
