#pragma once
#include"GameObject.h"
typedef std::vector<LPGAMEOBJECT> cellObjects;
#define CELL_SIZE  256

class Grid
{
	int mapWidth;
	int mapHeight;
	int gridCol;
	int gridRow;
	int cellSize = CELL_SIZE;

	std::vector<std::vector<cellObjects>> grid;
	std::vector<LPGAMEOBJECT> alwaysUpdateList;
public:
	Grid(int mapWidth, int mapHeight);
	// them object vao grid
	void Add(LPGAMEOBJECT object, bool isAlwayUpdateObject = false);
	void Update(LPGAMEOBJECT object);
	void GetListobjectFromGrid(vector<LPGAMEOBJECT>& listobjects);
};


