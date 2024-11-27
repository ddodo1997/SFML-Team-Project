#include "stdafx.h"
#include "TileMapEditor.h"

TileMapEditor::TileMapEditor(const std::string& name)
	: GameObject(name)
{
}

void TileMapEditor::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void TileMapEditor::SetRotation(float angle)
{
	rotation = angle;
}

void TileMapEditor::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void TileMapEditor::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void TileMapEditor::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void TileMapEditor::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void TileMapEditor::Release()
{
}

void TileMapEditor::Reset()
{
	texture = &TEXTURE_MGR.Get(tileMapTexId);
	int numTilesX = texture->getSize().x / 16;
	int numTilesY = texture->getSize().y / 16;
	tileSelector.setPrimitiveType(sf::Quads);
	tileSelector.resize(numTilesX * numTilesY * 4);

	sf::Vector2f tilePosOffset[4] =
	{
		{0.f, 0.f},
		{16.f, 0.f},
		{16.f, 16.f},
		{0.f, 16.f}
	};

	for (int i = 0; i < numTilesY; i++)
	{
		for (int j = 0; j < numTilesX; j++)
		{
			int quadIndex = i * numTilesX + j;
			sf::Vector2f quadPos(j * 16.f, i * 16.f);
			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = quadIndex * 4 + k;
				tileSelector[vertexIndex].position = quadPos + tilePosOffset[k];
				tileSelector[vertexIndex].texCoords = quadPos + tilePosOffset[k];
			}
		}
	}
	selectedTileSprite.setTexture(*texture);
	selectedTileSprite.setScale(1.f, 1.f);
}

void TileMapEditor::Update(float dt)
{
	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		sf::Vector2f worldPos = static_cast<sf::Vector2f>(mousePos);

		if (IsInTileSelectArea(worldPos))
		{
			SetSelectTileIndex(GetSelectTileIndex(worldPos));
		}
	}
	selectedTileSprite.setPosition(static_cast<sf::Vector2f>(mousePos));
}

void TileMapEditor::Draw(sf::RenderWindow& window)
{
	sf::RenderStates states;
	states.texture = texture;
	window.draw(tileSelector, states);

	if (selectedTileIndex != -1)
	{
		window.draw(selectedTileSprite);
	}
}

int TileMapEditor::GetSelectTileIndex(const sf::Vector2f& position)
{
	int xIndex = static_cast<int>(position.x) / 16;
	int yIndex = static_cast<int>(position.y) / 16;

	std::cout << yIndex * (texture->getSize().x / 16) + xIndex << std::endl;
	return yIndex * (texture->getSize().x / 16) + xIndex;
}

bool TileMapEditor::IsInTileSelectArea(const sf::Vector2f& position) const
{
	return (position.x >= 0 && position.x < texture->getSize().x && position.y >= 0 && position.y < texture->getSize().y);
}

void TileMapEditor::SetSelectTileIndex(int index)
{
	selectedTileIndex = index;
	int numTilesX = texture->getSize().x / 16;
	int xIndex = index % numTilesX;
	int yIndex = index / numTilesX;

	selectedTileSprite.setTextureRect(sf::IntRect(xIndex * 16, yIndex * 16, 16, 16));
	selectedTileSprite.setScale(2.f, 2.f);
}
