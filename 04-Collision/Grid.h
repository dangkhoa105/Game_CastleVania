#pragma once
#include"GameObject.h"
typedef std::vector<LPGAMEOBJECT> cellObjects;
#define CELL_SIZE  256

class Grid
{

	int gridCol;
	int gridRow;
	int cellSize = CELL_SIZE;

	std::vector<std::vector<cellObjects>> grid;
	std::vector<LPGAMEOBJECT> alwaysUpdateList;
	
public:
	Grid(int mapWidth, int mapHeight);
	Grid() {};
	int GetGridCol() { return gridCol; }
	// them object vao grid
	void Add(LPGAMEOBJECT object, bool isAlwayUpdateObject = false);
	void AddWhenUpdate(LPGAMEOBJECT object);
	void Update(LPGAMEOBJECT object);
	void GetListobjectFromGrid(vector<LPGAMEOBJECT>& listobjects);
	void LoadGridObject(string filePath);
};


