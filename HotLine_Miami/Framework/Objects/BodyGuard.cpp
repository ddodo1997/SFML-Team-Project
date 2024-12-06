#include "stdafx.h"
#include "BodyGuard.h"
#include "Player.h"

BodyGuard::BodyGuard(const std::string& name)
	: GameObject(name)
{
}
void BodyGuard::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void BodyGuard::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void BodyGuard::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void BodyGuard::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void BodyGuard::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void BodyGuard::SetPlayer(Player* player)
{
	this->player = player;
}

void BodyGuard::Init()
{
}

void BodyGuard::Release()
{
}

void BodyGuard::Reset()
{
	animator.SetTarget(&body);
	animator.Play("animations/Boss2/boss2_bodyguard_idle.json");
	SetOrigin(Origins::MC);
}

void BodyGuard::Update(float dt)
{
	animator.Update(dt);
}

void BodyGuard::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
