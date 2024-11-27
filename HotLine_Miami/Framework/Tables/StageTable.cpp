#include "stdafx.h"
#include "StageTable.h"

bool StageTable::Load()
{
	Release();
	std::ifstream inFile("tables/test_stage.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}

	json data;
	inFile >> data;

	tileSize = { data["tileSizeX"], data["tileSizeY"] };
	tileCount = { data["tileCountX"], data["tileCountY"] };
	tileTextureId = data["floor"]["textureId"];
	floorTiles = data["floor"]["tiles"].get<std::vector<int>>();
	wallIds = data["walls"].get<std::vector<std::string>>();
	enemyIds = data["enemies"].get<std::vector<std::string>>();
	decoTextureId = data["decorations"]["textureId"];

	for (const auto& decoration : data["decorations"]["decos"])
	{
		DataDecoration decoData;
		decoData.id = decoration["id"];
		decoTable[decoData.id].id = decoData.id;
		decoTable[decoData.id].pos = { decoration["x"], decoration["y"] };
		decoTable[decoData.id].rotation = decoration["rotation"];
		decoTable[decoData.id].type = decoration["type"];
		decoTable[decoData.id].size = { decoration["width"], decoration["height"] };
	}
	inFile.close();
	return true;
}

void StageTable::Release()
{
	floorTiles.clear();
	wallIds.clear();
	enemyIds.clear();
	decoTable.clear();
}
