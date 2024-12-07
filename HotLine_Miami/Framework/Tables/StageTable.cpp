#include "stdafx.h"
#include "StageTable.h"
#include "WallTable.h"
#include "WeaponTable.h"
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
		enemyData.rotation = enemy["rotation"];

		const auto& enemyTypeInfo = ENEMY_TABLE->GetEnemyType(enemy["state"]);

		enemyData.state = enemyTypeInfo.state;
		enemyData.weaponType = enemyTypeInfo.weaponType;
		
		for (const auto& waypoint : enemy["waypoint"])
		{
			enemyData.waypoints.emplace_back((waypoint["x"] * GetTileSize().x) + GetTileSize().x * 0.5f, (waypoint["y"] * GetTileSize().y) + GetTileSize().y * 0.5f);
		}

		enemyTable.insert({ enemyData.id, enemyData });
	}

	for (const auto& wall : data["wall"]["walls"])
	{
		DataWall wallData;
		wallData.id = wall["id"];
		wallData.start = { wall["start"]["x"], wall["start"]["y"] };
		wallData.end = { wall["end"]["x"], wall["end"]["y"] };
		for (const auto& textureId : wall["ids"].get<std::vector<std::string>>())
		{
			const auto& wallTypeData = WALL_TABLE->GetWall(textureId);

			if (wallTypeData.type == Wall::Types::Horizontal)
			{
				wallData.textureIds.push_back(WALL_TABLE->GetHorizontalWallTextureId(textureId));
			}
			else if (wallTypeData.type == Wall::Types::Vertical)
			{
				wallData.textureIds.push_back(WALL_TABLE->GetVerticalWallTextureId(textureId));
			}
		}
		wallTable.insert({ wallData.id, wallData });
	}

	for (const auto& weapon : data["weapon"]["weapons"])
	{
		DataWeapon weaponData;
		weaponData.id = weapon["id"];
		weaponData.pos = { weapon["x"], weapon["y"] };
		weaponData.rotation = weapon["rotation"];
		Weapon::WeaponType temp = weapon["type"];
		weaponData.weaponState = WEAPON_TABLE->Get(temp);

		weaponTable.insert({ weaponData.id, weaponData });
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
	weaponTable.clear();
}
