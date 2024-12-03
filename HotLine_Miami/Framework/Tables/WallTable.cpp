#include "stdafx.h"
#include "WallTable.h"
DataWallType WallTable::Undefined;
bool WallTable::Load()
{
	Release();
	std::ifstream inFile("tables/wall.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}

	json data;
	inFile >> data;

	filePath = data["filePath"];

	for (const auto& wall : data["walls"])
	{
		DataWallType dataWallType;
		dataWallType.id = wall["id"];
		dataWallType.textureId = wall["textureId"];
		dataWallType.type = StringToWallType(wall["type"]);
		table.insert({ dataWallType.id, dataWallType });

		if (dataWallType.type == Wall::Types::Horizontal)
		{
			horizontalWallTextures[dataWallType.id] = dataWallType.textureId;
		}
		else
		{
			verticalWallTextures[dataWallType.id] = dataWallType.textureId;
		}
	}

	inFile.close();
	return true;
}

void WallTable::Release()
{
	table.clear();
}

const DataWallType& WallTable::GetWall(const std::string& id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}

	return find->second;
}

Wall::Types WallTable::StringToWallType(const std::string& type)
{
	auto it = stringToWallType.find(type);
	if (it != stringToWallType.end())
	{
		return it->second;
	}

	return Wall::Types();
}

const std::string WallTable::GetHorizontalWallTextureId(const std::string& id)
{
	auto it = horizontalWallTextures.find(id);
	if (it != horizontalWallTextures.end())
	{
		return it->second;
	}

	return "";
}

const std::string WallTable::GetVerticalWallTextureId(const std::string& id)
{
	auto it = verticalWallTextures.find(id);
	if (it != verticalWallTextures.end())
	{
		return it->second;
	}

	return "";
}



