#pragma once

struct Alphabets
{
	char alphabet;
	sf::IntRect texCoord;
	int characterWidth;
};

class AlphabetTable : public DataTable
{
public:
	static Alphabets Undefined;

protected:
	std::unordered_map<char, Alphabets> table;
	std::unordered_map<char, Alphabets> pauseTable;
	std::string fontSpritePath;
	std::string pauseFontSpritePath;
	std::string filePath = "tables/alphabetTable.json";
	std::string filePath2 = "tables/alphabetTable2.json";

public:
	AlphabetTable() : DataTable(DataTable::Types::Alphabet) { };
	~AlphabetTable() = default;

	bool Load() override;
	void Release() override;
	void SetPath();

	const Alphabets& Get(char& id, bool isPause = false);
	const std::string& GetSpirtePath(bool isPause = false);
};