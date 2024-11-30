#pragma once
#include "DataTable.h"
#include <Enemy.h>
struct DataEnemyType
{
	std::string id = "";
	std::string weaponType = "";
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
public:
	static DataEnemyType Undefined;

	EnemyTable() : DataTable(DataTable::Types::Enemies) { };
	~EnemyTable() = default;

	bool Load() override;
	void Release() override;

	const DataEnemyType& GetEnemyType(const std::string& id) const;
	Enemy::Status StringToEnemyStatus(const std::string& status);
};
