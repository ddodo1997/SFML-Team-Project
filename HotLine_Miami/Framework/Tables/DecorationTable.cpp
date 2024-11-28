#include "stdafx.h"
#include "DecorationTable.h"
DataDecoration DecorationTable::Undefined;
bool DecorationTable::Load()
{
	Release();
	std::ifstream inFile("tables/decorations.json");

	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << std::endl;
		return false;
	}

	json data;
	inFile >> data;

	filePath = data["filePath"];
	for (const auto& decoration : data["decorations"])
	{
		DataDecoration decoData;
		decoData.id = decoration["id"];
		decoData.textureId = decoration["textureId"];
		decoData.size = { decoration["width"], decoration["height"] };

		table.insert({ decoData.id, decoData });
	}

	inFile.close();
	return true;
}

void DecorationTable::Release()
{
	table.clear();
}

const DataDecoration& DecorationTable::GetDecoration(const std::string& id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}

	return find->second;
}
