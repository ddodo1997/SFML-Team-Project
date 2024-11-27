#include "stdafx.h"
#include "FloorTable.h"

bool FloorTable::Load()
{
	Release();
	std::ifstream inFile("tables/test_floor.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}
	 
	json data;
	inFile >> data;

	tileSize = { data["floor"]["TileSizeX"], data["floor"]["TileSizeY"] };
	tileCount = { data["floor"]["TileCountX"], data["floor"]["TileCountY"] };
	tiles = data["floor"]["tiles"].get<std::vector<int>>();
	textureId = data["floor"]["textureId"];

	inFile.close();
	return true;
}

void FloorTable::Release()
{
	tiles.clear();
}
