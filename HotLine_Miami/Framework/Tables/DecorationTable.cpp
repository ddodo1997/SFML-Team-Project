#include "stdafx.h"
#include "DecorationTable.h"

bool DecorationTable::Load()
{
	Release();
	std::ifstream inFile("tables/test_deco.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}

	json data;
	inFile >> data;

	textureId = data["textureId"];
	for (const auto& decoration : data["decorations"])
	{
		DataDecoration dataDeco;
		dataDeco.id = decoration["id"];
		dataDeco.position = { decoration["x"], decoration["y"] };
		dataDeco.size = { decoration["width"], decoration["height"] };
		dataDeco.rotation = decoration["rotation"];
		dataDeco.type = decoration["type"];
		table[dataDeco.id] = dataDeco;
	}

	inFile.close();
	return true;
}

void DecorationTable::Release()
{
	table.clear();
}
