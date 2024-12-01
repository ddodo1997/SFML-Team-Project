#include "stdafx.h"
#include "EnemyTable.h"
DataEnemyType EnemyTable::Undefined;
bool EnemyTable::Load()
{
	Release();
	std::ifstream inFile("tables/enemy.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}

	json data;
	inFile >> data;

	for (const auto& enemy : data["enemies"])
	{
		DataEnemyType dataEnemyType;
		dataEnemyType.id = enemy["id"];
		dataEnemyType.weaponType = StringToWeaponType(enemy["weapontype"]);
		dataEnemyType.state = StringToEnemyStatus(enemy["state"]);

		table[dataEnemyType.id] = dataEnemyType;
	}

	inFile.close();
	return true;
}

void EnemyTable::Release()
{
	table.clear();
}

const DataEnemyType& EnemyTable::GetEnemyType(const std::string& id) const
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}

	return find->second;
}

Enemy::Status EnemyTable::StringToEnemyStatus(const std::string& status)
{
	auto it = stringToStatusMap.find(status);
	if (it != stringToStatusMap.end())
	{
		return it->second;
	}

	return Enemy::Status();
}

Weapon::WeaponType EnemyTable::StringToWeaponType(const std::string& type)
{
	auto it = stringToWeaponMap.find(type);
	if (it != stringToWeaponMap.end())
	{
		return it->second;
	}

	return Weapon::WeaponType();
}
