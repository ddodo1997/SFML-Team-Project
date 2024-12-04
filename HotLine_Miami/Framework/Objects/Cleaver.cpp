#include "stdafx.h"
#include "Cleaver.h"
#include "SceneGame.h"
#include "Player.h"
#include "Boss1.h"
Cleaver::Cleaver(const std::string& name)
	: GameObject(name)
{
}

void Cleaver::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Cleaver::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Cleaver::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Cleaver::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Cleaver::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Cleaver::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Cleaver::Release()
{
}

void Cleaver::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = sceneGame->GetPlayer();
	boss = sceneGame->GetBoss1();
	body.setTexture(TEXTURE_MGR.Get("graphics/Enemy/Boss1/boss1_cleaver.png"));
	SetOrigin(Origins::MC);
	SetActive(false);
	isStuck = false;
}

void Cleaver::Update(float dt)
{
	if (!isStuck)
	{
		SetPosition(position + direction * speed * dt);
		SetRotation(rotation + 1000.f * dt);
	}

	if (player->GetCollisionBox().getGlobalBounds().intersects(body.getGlobalBounds()))
	{
		player->OnHit(boss->GetWeaponStatus(), boss->GetDirection());
		SetActive(false);
	}
}

void Cleaver::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Cleaver::Throw(sf::Vector2f direction, sf::Vector2f pos)
{
	this->direction = direction;
	SetPosition(pos);
	speed = 400.f;
	isStuck = false;
	SetActive(true);
}

void Cleaver::OnPickUp()
{
	SetActive(false);
}

void Cleaver::OnStuckWall()
{
	isStuck = true;
}
