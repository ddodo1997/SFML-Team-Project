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


	for (const auto& decoration : data["decorations"]["decos"])
	{
		DataDecoration decoData;
		decoData.id = decoration["id"];
		decoData.pos = { decoration["x"], decoration["y"] };
		decoData.rotation = decoration["rotation"];

		const auto& decoInfo = DECORATION_TABLE->GetDecoration(decoData.id);

		decoData.textureId = DECORATION_TABLE->GetFilePath() + decoInfo.textureId;
		decoData.size = decoInfo.size;

		decoTable.insert({ decoData.id, decoData });
	}

	for (const auto& enemy : data["enemy"]["enemies"])
	{
		DataEnemy enemyData;
		enemyData.id = enemy["id"];
		enemyData.pos = { enemy["x"], enemy["y"] };

		const auto& enemyInfo = ENEMY_TABLE->GetEnemy(enemyData.id);

		enemyData.state = enemyInfo.state;
		enemyData.weaponType = enemyInfo.weaponType;

		for (const auto& waypoint : enemy["waypoint"])
		{
			enemyData.waypoints.emplace_back(waypoint["x"], waypoint["y"]);
		}

		enemyTable.insert({ enemyData.id, enemyData });
	}

	for (const auto& wall : data["wall"]["walls"])
	{
		DataWall wallData;
		wallData.id = wall["id"];
		wallData.start = { wall["start"]["x"], wall["start"]["y"] };
		wallData.end = { wall["end"]["x"], wall["end"]["y"] };
		wallData.textureIds = wall["ids"].get<std::vector<std::string>>();
		wallTable.insert({ wallData.id, wallData });
	}

	inFile.close();
	return true;
}

void StageTable::Release()
{
	floorTiles.clear();
	enemyTable.clear();
	decoTable.clear();
	wallTable.clear();
}
