#pragma once
#include "DataTable.h"
#include <Enemy.h>
struct DataEnemy
{
	std::string id = "";
	sf::Vector2f pos = { 0.f, 0.f };
	std::string weaponType = "";
	Enemy::Status state = Enemy::Status::Normal;
	std::vector<sf::Vector2f> waypoints;
};
class EnemyTable : public DataTable
{
private:
	std::unordered_map<std::string, DataEnemy> table;
	std::unordered_map<std::string, Enemy::Status> stringToStatusMap = {
		{"Normal", Enemy::Status::Normal},
		{"Idle", Enemy::Status::Idle},
		{"Patrol", Enemy::Status::Patrol}
	};
public:
	static DataEnemy Undefined;

	EnemyTable() : DataTable(DataTable::Types::Enemies) { };
	~EnemyTable() = default;

	bool Load() override;
	void Release() override;

	const DataEnemy& GetEnemy(const std::string& id) const;
	Enemy::Status StringToEnemyStatus(const std::string& status);
};
