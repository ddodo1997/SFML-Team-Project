#pragma once
#include "DataTable.h"

class FloorTable : public DataTable
{
private:
	sf::Vector2i tileSize;
	sf::Vector2i tileCount;
	std::vector<int> tiles;
	std::string textureId;
public:
	FloorTable() : DataTable(DataTable::Types::Floors) { };
	~FloorTable() = default;

	bool Load() override;
	void Release() override;
};

