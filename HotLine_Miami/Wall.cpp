#include "stdafx.h"
#include "Wall.h"
#include "WallTable.h"

Wall::Wall(const std::string& name)
{
}

void Wall::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	//UpdateTransform();
}

void Wall::SetRotation(float angle)
{
	rotation = angle;
	//UpdateTransform();
}

void Wall::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	//UpdateTransform();
}

void Wall::SetOrigin(Origins preset)
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

void Wall::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	// UpdateTransform();
}

void Wall::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Wall::Release()
{
}

void Wall::Reset()
{
	SetOrigin(Origins::TL);
	SetPosition({ 0.f, 0.f });
	SetScale({ 1.f, 1.f });
}

void Wall::Update(float dt)
{
}

void Wall::Draw(sf::RenderWindow& window)
{
	UpdateTransform();

	for (int i = 0; i < va.getVertexCount(); i += 4)
	{
		sf::RenderStates states;
		const sf::Texture* currentTexture = textures[(i / 4) % textures.size()];
		states.texture = currentTexture;
		window.draw(&va[i], 4, sf::Quads, states);
	}
}

void Wall::SetTexture(const std::string& id)
{
}

void Wall::DrawWall(const DataWall& dataWall)
{
	data = dataWall;
	position = data.start;
	length = (data.start.x == data.end.x) ? abs(data.end.y - data.start.y) * 16.f : abs(data.end.x - data.start.x) * 16.f;
	if (data.start.x == data.end.x)
	{
		SetOrigin(Origins::TC);
	}
	else
	{
		SetOrigin(Origins::ML);
	}

	textures.clear();
	for (const auto& textureId : data.textureIds)
	{
		textures.push_back(&TEXTURE_MGR.Get(WALL_TABLE->GetFilePath() + textureId));
	}

	if (textures.empty()) return;

	sf::Vector2f texSize(textures[0]->getSize().x, textures[0]->getSize().y);
	int quadCount = (data.start.x == data.end.x) ? static_cast<int>(length / texSize.y) : static_cast<int>(length / texSize.x);

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(quadCount * 4);

	for (int i = 0; i < quadCount; ++i)
	{
		sf::Vector2f offset = (data.start.x == data.end.x) ? sf::Vector2f(0.f, i * texSize.y) : sf::Vector2f(i * texSize.x, 0.f);

		int vertexIndex = i * 4;
		va[vertexIndex].position = position + offset;
		va[vertexIndex + 1].position = position + offset + sf::Vector2f(texSize.x, 0.f);
		va[vertexIndex + 2].position = position + offset + sf::Vector2f(texSize.x, texSize.y);
		va[vertexIndex + 3].position = position + offset + sf::Vector2f(0.f, texSize.y);

		const sf::Texture* currentTexture = textures[i % textures.size()];
		sf::Vector2f currentTexSize(currentTexture->getSize().x, currentTexture->getSize().y);

		va[vertexIndex].texCoords = { 0.f, 0.f };
		va[vertexIndex + 1].texCoords = { currentTexSize.x, 0.f };
		va[vertexIndex + 2].texCoords = { currentTexSize.x, currentTexSize.y };
		va[vertexIndex + 3].texCoords = { 0.f, currentTexSize.y };
	}

	UpdateTransform();
}

void Wall::UpdateTransform()
{
	transform = sf::Transform::Identity;
	transform.translate(position);
	transform.rotate(rotation);
	transform.scale(scale);
	transform.translate(-origin);
}