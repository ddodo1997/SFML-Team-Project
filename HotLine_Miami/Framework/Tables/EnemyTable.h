#pragma once
#include "DataTable.h"
#include <Enemy.h>
struct DataEnemyType
{
	std::string id = "";
	Weapon::WeaponType weaponType = Weapon::WeaponType::None;
	Enemy::Status state = Enemy::Status::Normal;
};
class EnemyTable : public DataTable
{
private:
	std::unordered_map<std::string, DataEnemyType> table;
	std::unordered_map<std::string, Enemy::Status> stringToStatusMap = {
		{"Normal", Enemy::Status::Normal},
		{"Idle", Enemy::Status::Idle},
		{"Patrol", Enemy::Status::Patrol}
	};
	std::unordered_map<std::string, Weapon::WeaponType> stringToWeaponMap = {
		{"None", Weapon::WeaponType::None},
		{"Bat", Weapon::WeaponType::Bat},
		{"Knife", Weapon::WeaponType::Knife},
		{"Machinegun", Weapon::WeaponType::Machinegun},
		{"Shotgun", Weapon::WeaponType::Shotgun}
	};
public:
	static DataEnemyType Undefined;

	EnemyTable() : DataTable(DataTable::Types::Enemies) { };
	~EnemyTable() = default;

	bool Load() override;
	void Release() override;

	const DataEnemyType& GetEnemyType(const std::string& id) const;
	Enemy::Status StringToEnemyStatus(const std::string& status);
	Weapon::WeaponType StringToWeaponType(const std::string& type);
};
