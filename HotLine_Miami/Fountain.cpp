#include "stdafx.h"
#include "Fountain.h"

Fountain::Fountain(const std::string& name)
	: GameObject(name)
{
}

void Fountain::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Fountain::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Fountain::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Fountain::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Fountain::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Fountain::Init()
{
}

void Fountain::Release()
{
}

void Fountain::Reset()
{
	animator.SetTarget(&body);
	animator.Play("animations/Boss2/boss2_fountain.json");
	SetOrigin(Origins::MC);
}

void Fountain::Update(float dt)
{
	animator.Update(dt);
}

void Fountain::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
