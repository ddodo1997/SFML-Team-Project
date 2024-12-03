#pragma once
#include "DataTable.h"
#include "Wall.h"
struct DataWallType
{
	std::string id = "";
	std::string textureId = "";
	Wall::Types type = Wall::Types();
};
class WallTable : public DataTable
{
public:

private:
	std::unordered_map<std::string, DataWallType> table;
	std::unordered_map<std::string, std::string> horizontalWallTextures;
	std::unordered_map<std::string, std::string> verticalWallTextures;
	std::unordered_map<std::string, Wall::Types> stringToWallType = {
		{"Horizontal", Wall::Types::Horizontal},
		{"Vertical",Wall::Types::Vertical}
	};
	std::string filePath;
public:
	static DataWallType Undefined;
	WallTable() : DataTable(DataTable::Types::Walls) { };
	~WallTable() = default;

	bool Load() override;
	void Release() override;

	const DataWallType& GetWall(const std::string& id);
	const std::string GetFilePath() { return filePath; }
	const std::unordered_map<std::string, DataWallType>& GetTable() { return table; }
	Wall::Types StringToWallType(const std::string& type);

	const std::string GetHorizontalWallTextureId(const std::string& id);
	const std::string GetVerticalWallTextureId(const std::string& id);

	const std::unordered_map<std::string, std::string>& GetHorizontalTable() { return horizontalWallTextures; }
	const std::unordered_map<std::string, std::string>& GetVerticalTable() { return verticalWallTextures; }
};

