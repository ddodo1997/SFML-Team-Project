#pragma once
#include "DataTable.h"

struct DataWall
{
	std::string id;
	sf::Vector2f pos;
	std::string type;
	std::string direction;
};

class WallTable : public DataTable
{
private:
	std::unordered_map<std::string, DataWall> table;
	std::string textureId;
public:
	static DataWall Undefined;

	WallTable() : DataTable(DataTable::Types::Walls) { };
	~WallTable() = default;

	bool Load() override;
	void Release() override;
	const DataWall& GetWall(const std::string& id) const;
	std::string GetTextureId() const { return textureId; }
};

