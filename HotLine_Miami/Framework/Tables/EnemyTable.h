#pragma once
#include "DataTable.h"
struct DataEnemy
{
	std::string id;
	sf::Vector2f pos;
	std::string weaponType;
	std::string state;
	std::vector<sf::Vector2i> wayPoints;
};
class EnemyTable : public DataTable
{
private:
	std::unordered_map<std::string, DataEnemy> table;
public:
	static DataEnemy Undefined;

	EnemyTable() : DataTable(DataTable::Types::Enemies) { };
	~EnemyTable() = default;

	bool Load() override;
	void Release() override;
	const DataEnemy& GetEnemy(const std::string& id) const;
};
