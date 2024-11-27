#include "stdafx.h"
#include "WallTable.h"
DataWall WallTable::Undefined;
bool WallTable::Load()
{
	Release();
	std::ifstream inFile("tables/test_wall.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}

	json data;
	inFile >> data;

	textureId = data["textureId"];
	for (const auto& wall : data["walls"])
	{
		DataWall dataWall;
		dataWall.id = wall["id"];
		dataWall.pos = { wall["x"], wall["y"] };
		dataWall.type = wall["type"];
		dataWall.direction = wall["direction"];
		table[dataWall.id] = dataWall;
	}

	inFile.close();
	return true;
}

void WallTable::Release()
{
	table.clear();
}

const DataWall& WallTable::GetWall(const std::string& id) const
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}

	return find->second;
}
