#pragma once
#include "DataTable.h"
struct DataDecoration
{
	std::string id = "";
	sf::Vector2f pos = { 0.f, 0.f };
	float rotation = 0.f;
	std::string textureId = "";
	sf::Vector2f size = { 0.f, 0.f };
};
class DecorationTable : public DataTable
{
private:
	std::unordered_map<std::string, DataDecoration> table;
	std::string filePath;
public:
	static DataDecoration Undefined;
	DecorationTable() : DataTable(DataTable::Types::Decos) { };
	~DecorationTable() = default;

	bool Load() override;
	void Release() override;
	const DataDecoration& GetDecoration(const std::string& id);
	const std::string GetFilePath() { return filePath; }
};

