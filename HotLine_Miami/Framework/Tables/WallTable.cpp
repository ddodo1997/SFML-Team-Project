#include "stdafx.h"
#include "WallTable.h"
DataWall WallTable::Undefined;
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
		std::string id = wall["id"];
		std::string textureId = wall["textureId"];
		table.insert({ id, textureId });
	}

	inFile.close();
	return true;
}

void WallTable::Release()
{
	table.clear();
}

const std::string& WallTable::GetWall(const std::string& id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return "";
	}

	return find->second;
}



