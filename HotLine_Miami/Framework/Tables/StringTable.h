#pragma once

class StringTable : public DataTable
{
public:
	static std::string Undefined;

protected:
	std::unordered_map<std::string, std::string> table;
	std::string filePath = "tables/main_string.json";
public:
	StringTable() : DataTable(DataTable::Types::String) { };
	~StringTable() = default;

	bool Load() override;
	void Release() override;
	void SetPath();

	const std::string& Get(const std::string& id);
};