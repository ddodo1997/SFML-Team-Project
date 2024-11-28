#pragma once
#include "DataTable.h"
#include "Wall.h"

class WallTable : public DataTable
{
private:
	std::unordered_map<std::string, std::string> table;
	std::string filePath;

	std::unordered_map<std::string, Wall::Types> stringToTypeMap = {
		{"Horizontal", Wall::Types::Horizontal},
		{"Vertical", Wall::Types::Vertical}
	};
public:
	static DataWall Undefined;
	WallTable() : DataTable(DataTable::Types::Walls) { };
	~WallTable() = default;

	bool Load() override;
	void Release() override;

	const std::string& GetWall(const std::string& id);
	const std::string GetFilePath() { return filePath; }
	Wall::Types StringToWallType(const std::string& type);
};

