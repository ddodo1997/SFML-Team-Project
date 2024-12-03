#include "stdafx.h"
#include "TileMapEditor.h"
#include "WallTable.h"
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
	currentMode = EditorMode::TileMode;
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

	const auto& wallDataList = WALL_TABLE->GetTable();
	for (const auto& wallData : wallDataList)
	{
		sf::Sprite wallSprite;
		wallSprite.setTexture(TEXTURE_MGR.Get(WALL_TABLE->GetFilePath() + wallData.second));
		wallSprite.setScale(2.f, 2.f);  
		wallSprite.setPosition(10.f + wallSprites.size() * 84.f, 10.f);

		wallSprites.insert({ wallData.first, wallSprite });
	}
	selectedWallSprite.setScale(1.f, 1.f);
	background.setSize(FRAMEWORK.GetWindowSizeF() * 0.4f);
	background.setFillColor(sf::Color(150, 150, 150, 100));
	background.setOutlineColor(sf::Color::White);
	background.setOutlineThickness(1.f);
}

void TileMapEditor::Update(float dt)
{
	switch (currentMode)
	{
	case EditorMode::TileMode:
		UpdateTileMode(dt);
		break;
	case EditorMode::WallMode:
		UpdateWallMode(dt);
		break;
	}

	UpdateSelectedSpritePosition();
}

void TileMapEditor::UpdateTileMode(float dt)
{
	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f worldPos = static_cast<sf::Vector2f>(mousePos);

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (IsInTileSelectArea(worldPos))
		{
			SetSelectTileIndex(GetSelectTileIndex(worldPos));
		}
	}
}

void TileMapEditor::UpdateWallMode(float dt)
{
	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f worldPos = static_cast<sf::Vector2f>(mousePos);

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		for (auto& pair : wallSprites)
		{
			if (pair.second.getGlobalBounds().contains(worldPos))
			{
				SetSelectWall(pair);
				break;
			}
		}
	}
}

void TileMapEditor::UpdateDecoMode(float dt)
{
}

void TileMapEditor::UpdateEnemyMode(float dt)
{
}

void TileMapEditor::UpdateSelectedSpritePosition()
{
	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	switch (currentMode)
	{
	case EditorMode::TileMode:
		if (selectedTileIndex != -1)
		{
			selectedTileSprite.setPosition({ mousePos.x - 20.f, mousePos.y - 20.f });
		}
		break;
	case EditorMode::WallMode:
		
		if (!selectedWallTextureId.empty())
		{
			selectedWallSprite.setPosition({ mousePos.x - 10.f, mousePos.y - 5.f });
		}
		break;
	}
}

void TileMapEditor::Draw(sf::RenderWindow& window)
{
	sf::RenderStates states;

	switch (currentMode)
	{
	case EditorMode::TileMode:
	{
		states.texture = texture;
		window.draw(background);
		window.draw(tileSelector, states);
		if (selectedTileIndex != -1)
		{
			window.draw(selectedTileSprite);
		}
		break;
	}
	case EditorMode::WallMode:
		window.draw(background);
		for (auto& pair : wallSprites)
		{
			window.draw(pair.second);
		}
		if (!selectedWallTextureId.empty())
		{
			window.draw(selectedWallSprite);
		}
		break;
	case EditorMode::DecorationMode:
		break;
	case EditorMode::EnemyMode:
		break;
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

void TileMapEditor::SetSelectWall(std::pair<std::string, sf::Sprite> wall)
{
	selectedWallTextureId = wall.first;
	selectedWallSprite = wallSprites[selectedWallTextureId];
	selectedWallSprite.setScale(2.f, 2.f);
	std::cout << selectedWallTextureId << std::endl;
}

void TileMapEditor::SetMode(EditorMode mode)
{
	currentMode = mode;
	selectedTileIndex = -1;
	selectedWallTextureId = "";
	switch (currentMode)
	{
	case EditorMode::TileMode:
	{
		selectedTileIndex = -1;
		background.setSize(FRAMEWORK.GetWindowSizeF() * 0.4f);
		break;
	}
	case EditorMode::WallMode:
		selectedWallTextureId = "";
		background.setSize({ FRAMEWORK.GetWindowSizeF().x * 0.5f, FRAMEWORK.GetWindowSizeF().y * 0.1f });
		break;
	case EditorMode::DecorationMode:
		break;
	case EditorMode::EnemyMode:
		break;
	}
}
