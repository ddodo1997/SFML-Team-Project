#include "stdafx.h"
#include "EnemyTable.h"
DataEnemy EnemyTable::Undefined;
bool EnemyTable::Load()
{
	Release();
	std::ifstream inFile("tables/test_enemy.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}

	json data;
	inFile >> data;

	for (const auto& enemy : data["enemies"])
	{
		DataEnemy dataEnemy;
		dataEnemy.id = enemy["id"];
		dataEnemy.pos = { enemy["x"], enemy["y"] };
		dataEnemy.weaponType = enemy["weapontype"];
		dataEnemy.state = enemy["state"];
		for (const auto& waypoint : enemy["waypoint"])
		{
			dataEnemy.wayPoints.push_back({ waypoint["x"], waypoint["y"] });
		}
		table[dataEnemy.id] = dataEnemy;
	}

	inFile.close();
	return true;
}

void EnemyTable::Release()
{
	table.clear();
}

const DataEnemy& EnemyTable::GetEnemy(const std::string& id) const
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}

	return find->second;
}
