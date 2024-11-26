#include "stdafx.h"
#include "StringTable.h"

std::string StringTable::Undefined = "Undefined Id";

bool StringTable::Load()
{
	SetPath();
	Release();
	std::ifstream fStream(filePath);

	if (!fStream.is_open())
	{
		std::cerr << "String Table File Open Error!!" << std::endl;
	}
	json data = json::parse(fStream);

	for (const auto& map : data)
	{
		table=map.get<std::unordered_map<std::string, std::string>>();
	}

	fStream.close();
	return true;
}

void StringTable::Release()
{
}

void StringTable::SetPath()
{
	switch (Variables::currentLang)
	{
	case Languages::English:
		filePath = "tables/main_string.json";
		break;
	}
}

const std::string& StringTable::Get(const std::string& id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}
	return find->second;
}