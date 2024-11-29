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

	states.texture = nullptr;
	window.draw(gridLines, states);
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

void TileMap::InitializeEmpty(const sf::Vector2i& tileSize, const sf::Vector2i& tileCount)
{
	this->tileSize = tileSize;
	this->tileCount = tileCount;
	floorTiles.assign(this->tileCount.x * this->tileCount.y, -1);
	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(this->tileCount.x * this->tileCount.y * 4);

	for (int y = 0; y < this->tileCount.y; ++y)
	{
		for (int x = 0; x < this->tileCount.x; ++x)
		{
			int quadIndex = (y * this->tileCount.x + x) * 4;

			sf::Vertex* quad = &va[quadIndex];

			sf::Vector2f quadPos(x * this->tileSize.x, y * this->tileSize.y);
			quad[0].position = quadPos;
			quad[1].position = { quadPos.x + this->tileSize.x, quadPos.y };
			quad[2].position = { quadPos.x + this->tileSize.x, quadPos.y + this->tileSize.y };
			quad[3].position = { quadPos.x, quadPos.y + this->tileSize.y };

			quad[0].texCoords = { 0.f, 0.f };
			quad[1].texCoords = { 0.f, 0.f };
			quad[2].texCoords = { 0.f, 0.f };
			quad[3].texCoords = { 0.f, 0.f };

			sf::Color gridColor = sf::Color (255, 255, 255, 255);
			quad[0].color = gridColor;
			quad[1].color = gridColor;
			quad[2].color = gridColor;
			quad[3].color = gridColor;
		}
	}

	gridLines.setPrimitiveType(sf::Lines);
	gridLines.resize((this->tileCount.x + 1) * this->tileCount.y + (this->tileCount.y + 1) * this->tileCount.x);

	int lineIndex = 0;
	for (int y = 0; y <= this->tileCount.y; ++y)
	{
		sf::Vector2f startPos(0.f, y * this->tileSize.y);
		sf::Vector2f endPos(this->tileCount.x * this->tileSize.x, y * this->tileSize.y);
		gridLines[lineIndex++] = sf::Vertex(startPos, sf::Color::White);
		gridLines[lineIndex++] = sf::Vertex(endPos, sf::Color::White);
	}

	for (int x = 0; x <= tileCount.x; ++x)
	{
		sf::Vector2f startPos(x * this->tileSize.x, 0.f);
		sf::Vector2f endPos(x * this->tileSize.x, this->tileCount.y * tileSize.y);
		gridLines[lineIndex++] = sf::Vertex(startPos, sf::Color::White);
		gridLines[lineIndex++] = sf::Vertex(endPos, sf::Color::White);
	}
}

void TileMap::PaintTile(const sf::Vector2f& mousePos, const int index)
{
	int xIndex = static_cast<int>(mousePos.x) / tileSize.x;
	int yIndex = static_cast<int>(mousePos.y) / tileSize.y;

	if (xIndex < 0 || yIndex < 0 || xIndex >= tileCount.x || yIndex >= tileCount.y)
	{
		return;
	}


	int tileIndex = yIndex * tileCount.x + xIndex;

	floorTiles[tileIndex] = index;

	int quadIndex = tileIndex * 4;
	sf::Vertex* quad = &va[quadIndex];

	int texX = (index % (texture->getSize().x / tileSize.x)) * tileSize.x;
	int texY = (index / (texture->getSize().x / tileSize.x)) * tileSize.y;

	quad[0].texCoords = { static_cast<float>(texX), static_cast<float>(texY) };
	quad[1].texCoords = { static_cast<float>(texX + tileSize.x), static_cast<float>(texY) };
	quad[2].texCoords = { static_cast<float>(texX + tileSize.x), static_cast<float>(texY + tileSize.y) };
	quad[3].texCoords = { static_cast<float>(texX), static_cast<float>(texY + tileSize.y) };
}
