#pragma once
#include "DataTable.h"
struct DataDecoration
{
	std::string id;
	sf::Vector2f position;
	sf::Vector2f size;
	float rotation;
	std::string type;
};
class DecorationTable : public DataTable
{
private:
	std::unordered_map<std::string, DataDecoration> table;
	std::string textureId;
public:
	DecorationTable() : DataTable(DataTable::Types::Decos) { };
	~DecorationTable() = default;

	bool Load() override;
	void Release() override;
};

