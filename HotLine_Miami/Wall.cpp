#include "stdafx.h"
#include "Wall.h"

Wall::Wall(const std::string& name)
{
}

void Wall::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(pos);
}

void Wall::SetRotation(float angle)
{
}

void Wall::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void Wall::SetOrigin(Origins preset)
{
}

void Wall::SetOrigin(const sf::Vector2f& newOrigin)
{
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
	SetPosition({ 0.f, 0.f });
	SetScale({ 1.f, 1.f });
}

void Wall::Update(float dt)
{
}

void Wall::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Wall::SetTexture(const std::string& id)
{
	sprite.setTexture(TEXTURE_MGR.Get(id));
}
