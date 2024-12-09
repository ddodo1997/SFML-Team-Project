#include "stdafx.h"
#include "StageTable.h"
#include "WallTable.h"
#include "WeaponTable.h"
bool StageTable::Load()
{
	Release();
	int i = 0;
	while(true)
	{
		std::ifstream inFile("tables/stage_" + std::to_string(i) + ".json");

		if (!inFile)
		{
			std::cerr << "JSON ������ �� �� �����ϴ�: " << "tables/stage_" + std::to_string(i) + ".json" << std::endl;
			break;
		}

		json data;
		inFile >> data;
		inFile.close();

		DataStage stage;
		tileSize = { data["tileSizeX"], data["tileSizeY"] };
		stage.tileSize = { data["tileSizeX"], data["tileSizeY"] };
		stage.tileCount = { data["tileCountX"], data["tileCountY"] };
		stage.tileTextureId = data["floor"]["textureId"];
		stage.floorTiles = data["floor"]["tiles"].get<std::vector<int>>();

		ParseDecorations(data["decorations"]["decos"], stage);
		ParseEnemies(data["enemy"]["enemies"], stage);
		ParseWalls(data["wall"]["walls"], stage);
		ParseWeapons(data["weapon"]["weapons"], stage);

		if (data.contains("player"))
		{
			stage.playerData.pos = { data["player"]["x"], data["player"]["y"] };
			stage.playerData.rotation = data["player"]["rotation"];
		}

		if (data.contains("boss1"))
		{
			stage.boss1Data.pos = { data["boss1"]["x"], data["boss1"]["y"] };
			stage.boss1Data.rotation = data["boss1"]["rotation"];
		}
		else
		{
			stage.boss1Data.pos = { -1.f, -1.f };
			stage.boss1Data.rotation = 0.0f;
		}

		if (data.contains("boss2"))
		{
			stage.boss2Position = { data["boss2"]["x"], data["boss2"]["y"] };
		}
		else
		{
			stage.boss2Position = { -1.f, -1.f };
		}

		for (const auto& pos : data["EndPosition"])
		{
			stage.endPoints.push_back({ pos["x"], pos["y"] });
		}

		stageDatas.push_back(stage);
		i++;
	}

	LoadStage();


	return !stageDatas.empty();
}

void StageTable::Release()
{
	for (auto& stage : stageDatas)
	{
		stage.decoTable.clear();
		stage.enemyTable.clear();
		stage.wallTable.clear();
		stage.weaponTable.clear();
		stage.floorTiles.clear();
		stage.endPoints.clear();

		stage.tileSize = { 0, 0 };
		stage.tileCount = { 0, 0 };
		stage.tileTextureId.clear();
		stage.playerData = DataPlayer();
		stage.boss1Data = DataBoss1();
		stage.boss2Position = { 0.f, 0.f };
	}

	stageDatas.clear();
	currentStageIndex = 0;
}

void StageTable::ParseDecorations(const json& decorations, DataStage& stage)
{
	for (const auto& decoration : decorations)
	{
		DataDecoration decoData;
		decoData.id = decoration["id"];
		decoData.pos = { decoration["x"], decoration["y"] };
		decoData.rotation = decoration["rotation"];

		const auto& decoInfo = DECORATION_TABLE->GetDecoration(decoData.id);

		decoData.textureId = DECORATION_TABLE->GetFilePath() + decoInfo.textureId;
		decoData.size = decoInfo.size;

		stage.decoTable.insert({ decoData.id, decoData });
	}
}

void StageTable::ParseWalls(const json& walls, DataStage& stage)
{
	for (const auto& wall : walls)
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
		stage.wallTable.insert({ wallData.id, wallData });
	}
}

void StageTable::ParseEnemies(const json& enemies, DataStage& stage)
{
	for (const auto& enemy : enemies)
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
			enemyData.waypoints.emplace_back((waypoint["x"] * stage.GetTileSize().x) + stage.GetTileSize().x * 0.5f, (waypoint["y"] * stage.GetTileSize().y) + stage.GetTileSize().y * 0.5f);
		}

		stage.enemyTable.insert({ enemyData.id, enemyData });
	}
}

void StageTable::ParseWeapons(const json& weapons, DataStage& stage)
{
	for (const auto& weapon : weapons)
	{
		DataWeapon weaponData;
		weaponData.id = weapon["id"];
		weaponData.pos = { weapon["x"], weapon["y"] };
		weaponData.rotation = weapon["rotation"];
		Weapon::WeaponType temp = weapon["type"];
		weaponData.weaponState = WEAPON_TABLE->Get(temp);

		stage.weaponTable.insert({ weaponData.id, weaponData });
	}
}

const DataStage& StageTable::GetCurrentStage() const
{
	if (currentStageIndex >= 0 && currentStageIndex < stageDatas.size())
	{
		return stageDatas[currentStageIndex];
	}
}

void StageTable::SetSavedStageIndex(int idx)
{
	if (idx >= 0 || idx < stageDatas.size())
	{
		savedStageIndex = idx;
	}
}

void StageTable::SetCurrentStageIndex(int idx)
{
	if (idx >= 0 || idx < stageDatas.size())
	{
		currentStageIndex = idx;
	}
}

void StageTable::LoadStage()
{
	std::ifstream saveFile("tables/save_data.json");
	int saveIndex = 0;

	json data;
	saveFile >> data;
	saveFile.close();

	savedStageIndex = data["save_stage"];
}

void StageTable::NextStage()
{
	if (currentStageIndex + 1 < stageDatas.size())
	{
		currentStageIndex++;
	}
}
