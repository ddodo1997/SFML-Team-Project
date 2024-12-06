#include "stdafx.h"
#include "Panther.h"
#include "Player.h"
Panther::Panther(const std::string& name)
	: GameObject(name)
{
}

void Panther::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Panther::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Panther::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Panther::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Panther::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Panther::SetPlayer(Player* player)
{
	this->player = player;
}

void Panther::Init()
{
}

void Panther::Release()
{
}

void Panther::Reset()
{
	animator.SetTarget(&body);
	animator.Play("animations/Boss2/boss2_panther_sit.json");
	SetOrigin(Origins::MC);
}

void Panther::Update(float dt)
{
	animator.Update(dt);
}

void Panther::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
