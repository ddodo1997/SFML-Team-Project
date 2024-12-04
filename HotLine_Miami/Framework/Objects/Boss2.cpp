#include "stdafx.h"
#include "Boss2.h"

Boss2::Boss2(const std::string& name)
	: GameObject(name)
{
}

void Boss2::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void Boss2::SetRotation(float angle)
{
	rotation = angle;
}

void Boss2::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void Boss2::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void Boss2::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void Boss2::Init()
{
}

void Boss2::Release()
{
}

void Boss2::Reset()
{
}

void Boss2::Update(float dt)
{
}

void Boss2::Draw(sf::RenderWindow& window)
{
}
