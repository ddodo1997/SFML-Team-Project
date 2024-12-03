#include "stdafx.h"
#include "Wall2.h"
#include "WallTable.h"
Wall2::Wall2(const std::string& name)
{
}

void Wall2::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Wall2::SetStartPostion(const sf::Vector2f& pos)
{
	startPosition = pos;
}

void Wall2::SetEndPosition(const sf::Vector2f& pos)
{
	endPosition = pos;
}

void Wall2::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Wall2::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(scale);
}

void Wall2::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Wall2::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Wall2::SetId(const std::string& id)
{
	this->id = id;
}

void Wall2::SetTexture(const std::string& textureId)
{
	body.setTexture(TEXTURE_MGR.Get(WALL_TABLE->GetFilePath() + textureId));
}

void Wall2::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 2;
}

void Wall2::Release()
{
}

void Wall2::Reset()
{
	SetScale({ 1.f, 1.f });
	SetOrigin(Origins::MC);
}

void Wall2::Update(float dt)
{
	hitBox.UpdateTr(body, GetLocalBounds());
}

void Wall2::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}
