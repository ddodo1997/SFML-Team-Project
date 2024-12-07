#include "stdafx.h"
#include "AlphabetTable.h"

Alphabets AlphabetTable::Undefined;

bool AlphabetTable::Load()
{
	SetPath();
	Release();
	std::ifstream fStream(filePath);


	if (!fStream.is_open())
	{
		std::cerr << "Alphabet Table File Open Error!!" << std::endl;
	}
	json data = json::parse(fStream);

	sf::Vector2i rectSize;
	rectSize.x = data["Width"];
	rectSize.y = data["Height"];
	fontSpritePath = data["TextureId"];

	for (const auto& alphabet : data["Alphabets"])
	{
		Alphabets newAlphabet;
		sf::IntRect textureRectPos;
		textureRectPos.width = rectSize.x;
		textureRectPos.height = rectSize.y;
		textureRectPos.left = alphabet["RectPos"][0];
		textureRectPos.top = alphabet["RectPos"][1];
		newAlphabet.texCoord = textureRectPos;
		std::string targetAlphabet = alphabet["Character"];
		char charAlphabet = targetAlphabet.front();
		newAlphabet.alphabet = charAlphabet;
		newAlphabet.characterWidth = alphabet["CharWidth"];
		table.insert({ charAlphabet, newAlphabet });
	}

	fStream.close();

	std::ifstream fStream2(filePath2);


	if (!fStream.is_open())
	{
		std::cerr << "Alphabet2 Table File Open Error!!" << std::endl;
	}
	json data2 = json::parse(fStream2);

	sf::Vector2i rectSize2;
	rectSize2.x = data2["Width"];
	rectSize2.y = data2["Height"];
	pauseFontSpritePath = data2["TextureId"];

	for (const auto& alphabet : data2["Alphabets"])
	{
		Alphabets newAlphabet;
		sf::IntRect textureRectPos;
		textureRectPos.width = rectSize2.x;
		textureRectPos.height = rectSize2.y;
		textureRectPos.left = alphabet["RectPos"][0];
		textureRectPos.top = alphabet["RectPos"][1];
		newAlphabet.texCoord = textureRectPos;
		std::string targetAlphabet = alphabet["Character"];
		char charAlphabet = targetAlphabet.front();
		newAlphabet.alphabet = charAlphabet;
		newAlphabet.characterWidth = alphabet["CharWidth"];
		pauseTable.insert({ charAlphabet, newAlphabet });
	}

	fStream2.close();

	return true;
}

void AlphabetTable::Release()
{
}

void AlphabetTable::SetPath()
{
	switch (Variables::currentLang)
	{
	case Languages::English:
		filePath = "tables/alphabetTable.json";
		break;
	}
}

const Alphabets& AlphabetTable::Get(char& id, bool isPause)
{
	if (!isPause)
	{
		auto find = table.find(id);
		if (find == table.end())
		{
			return Undefined;
		}
		return find->second;
	}
	else
	{
		auto find = pauseTable.find(id);
		if (find == pauseTable.end())
		{
			return Undefined;
		}
		return find->second;
	}
}

const std::string& AlphabetTable::GetSpirtePath(bool isPause)
{
	if (isPause)
		return pauseFontSpritePath;
	else
		return fontSpritePath;
}
