#include "stdafx.h"
#include "TileMap.h"

#include <fstream>

TileMap::TileMap(const std::string& name)
	: GameObject(name)
{
}

void TileMap::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	UpdateTransform();
}

void TileMap::SetRotation(float angle)
{
	rotation = angle;
	UpdateTransform();
}

void TileMap::SetScale(const sf::Vector2f& s)
{
	scale = s;
	UpdateTransform();
}

void TileMap::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		sf::FloatRect rect = GetGlobalBounds();
		origin.x = rect.width * ((int)preset % 3) * 0.5f;
		origin.y = rect.height * ((int)preset / 3) * 0.5f;
	}
	UpdateTransform();
}

void TileMap::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

sf::FloatRect TileMap::GetLocalBounds() const
{
	return { 0.f, 0.f, cellCount.x * cellSize.x, cellCount.y * cellSize.y };
}

sf::FloatRect TileMap::GetGlobalBounds() const
{
	sf::FloatRect bounds = GetLocalBounds();
	return transform.transformRect(bounds);
}

sf::Vector2f TileMap::GetCellSize() const
{
	return cellSize;
}

void TileMap::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = -1;
}

void TileMap::Release()
{
}

void TileMap::Reset()
{
	texture = &TEXTURE_MGR.Get(tileMapTexId);

	LoadMapJson("tables/maptable.json");
	SetOrigin(originPreset);
	SetScale({ 1.0f, 1.0f });
	SetPosition({ 0.f, 0.f });
	cellSize = { 16.f, 16.f };
}

void TileMap::Update(float dt)
{
}

void TileMap::Draw(sf::RenderWindow& window)
{
	UpdateTransform();

	sf::RenderStates states;
	states.texture = texture;
	states.transform = transform;
	window.draw(va, states);
}

void TileMap::Set(const sf::Vector2i& count)
{
	cellCount = count;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4);
}

void TileMap::LoadMapJson(const std::string& filename)
{
	std::ifstream inFile(filename);
	if (!inFile)
	{
		std::cerr << "JSON 파일을 열 수 없습니다: " << filename << std::endl;
		return;
	}

	json js;
	inFile >> js;

	sf::Vector2i count = { js["width"], js["height"] };
	Set(count);

	texture = &TEXTURE_MGR.Get(tileMapTexId);
	sf::Vector2f tilePosOffset[4] =
	{
		{0.f, 0.f},
		{cellSize.x, 0.f},
		{cellSize.x, cellSize.y},
		{0.f, cellSize.y}
	};
	for (int i = 0; i < count.y; ++i)
	{
		for (int j = 0; j < count.x; ++j)
		{
			int tileIndex = js["tiles"][i * count.x + j];
			
			if (tileIndex < 0)
			{
				continue;
			}
			int quadIndex = i * count.x + j;
			sf::Vector2f quadPos(j * cellSize.x, i * cellSize.y);

			int texX = (tileIndex % 51) * static_cast<int>(cellSize.x);
			int texY = (tileIndex / 51) * static_cast<int>(cellSize.y);

			sf::Vector2f texCoords[4] =
			{
				{static_cast<float>(texX), static_cast<float>(texY)},
				{static_cast<float>(texX + cellSize.x), static_cast<float>(texY)},
				{static_cast<float>(texX + cellSize.x), static_cast<float>(texY + cellSize.y)},
				{static_cast<float>(texX), static_cast<float>(texY + cellSize.y)}
			};

			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = quadIndex * 4 + k;
				va[vertexIndex].position = quadPos + tilePosOffset[k];
				va[vertexIndex].texCoords = texCoords[k];
			}
		}
	}
}

void TileMap::UpdateTransform()
{
	transform = sf::Transform::Identity;
	transform.translate(position);
	transform.rotate(rotation);
	transform.scale(scale);
	transform.translate(-origin);
}
