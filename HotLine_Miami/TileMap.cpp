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
	return { 0.f, 0.f,  static_cast<float>(tileCount.x * tileSize.x), static_cast<float>(tileCount.y * tileSize.y) };
}

sf::FloatRect TileMap::GetGlobalBounds() const
{
	sf::FloatRect bounds = GetLocalBounds();
	return transform.transformRect(bounds);
}

sf::Vector2i TileMap::GetCellSize() const
{
	return tileSize;
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
	SetOrigin(originPreset);
	SetScale({ 1.0f, 1.0f });
	SetPosition({ 0.f, 0.f });
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

void TileMap::Initialize(const sf::Vector2i& tileSize, const sf::Vector2i& tileCount, const std::vector<int>& floorTiles)
{
	this->tileSize = tileSize;
	this->tileCount = tileCount;
	this->floorTiles = floorTiles;
	
	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(tileCount.x * tileCount.y * 4);

	for (int y = 0; y < tileCount.y; ++y)
	{
		for (int x = 0; x < tileCount.x; ++x)
		{
			int tileIndex = floorTiles[y * tileCount.x + x];

			if (tileIndex < 0)
			{
				continue;
			}

			int quadIndex = (y * tileCount.x + x) * 4;

			sf::Vertex* quad = &va[quadIndex];

			sf::Vector2f quadPos(x * tileSize.x, y * tileSize.y);
			quad[0].position = quadPos;
			quad[1].position = { quadPos.x + tileSize.x, quadPos.y };
			quad[2].position = { quadPos.x + tileSize.x, quadPos.y + tileSize.y };
			quad[3].position = { quadPos.x, quadPos.y + tileSize.y };
			
			int texX = (tileIndex % (texture->getSize().x / tileSize.x)) * tileSize.x;
			int texY = (tileIndex / (texture->getSize().x / tileSize.x)) * tileSize.y;

			quad[0].texCoords = { static_cast<float>(texX), static_cast<float>(texY) };
			quad[1].texCoords = { static_cast<float>(texX + tileSize.x), static_cast<float>(texY) };
			quad[2].texCoords = { static_cast<float>(texX + tileSize.x), static_cast<float>(texY + tileSize.y) };
			quad[3].texCoords = { static_cast<float>(texX), static_cast<float>(texY + tileSize.y) };
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
