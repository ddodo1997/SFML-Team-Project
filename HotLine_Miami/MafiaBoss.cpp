#include "stdafx.h"
#include "MafiaBoss.h"
#include "Player.h"

MafiaBoss::MafiaBoss(const std::string& name)
	: GameObject(name)
{
}

void MafiaBoss::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	legs.setPosition(position);
}

void MafiaBoss::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void MafiaBoss::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void MafiaBoss::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		Utils::SetOrigin(legs, originPreset);
	}
}

void MafiaBoss::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	legs.setOrigin(origin);
}

void MafiaBoss::SetPlayer(Player* player)
{
	this->player = player;
}

void MafiaBoss::Init()
{
}

void MafiaBoss::Release()
{
}

void MafiaBoss::Reset()
{
	animatorBody.SetTarget(&body);
	animatorBody.Play("animations/Boss2/boss2_idle.json");
	legs.setTexture(TEXTURE_MGR.Get("graphics/Enemy/Boss2/boss2_legs.png"));
	SetOrigin(Origins::MC);
}

void MafiaBoss::Update(float dt)
{
	animatorBody.Update(dt);
}

void MafiaBoss::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
