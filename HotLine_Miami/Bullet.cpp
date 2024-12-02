#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Wall2.h"
#include "SceneDev_K.h"
#include "SceneDevL.h"
#include "SceneGame.h"
Bullet::Bullet(const std::string& name)
	: GameObject(name)
{
}

void Bullet::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Bullet::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Bullet::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Bullet::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Bullet::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Bullet::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 3;
}

void Bullet::Release()
{
}

void Bullet::Reset()
{
	walls = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetWalls();
	animator.SetTarget(&body);
	animator.Play("animations/bullet.json");
}

void Bullet::Update(float dt)
{
	SetPosition(position + direction * speed * dt);
}

void Bullet::FixedUpdate(float dt)
{
	auto bulletBounds = body.getGlobalBounds();

	//º® 
	for (auto wall : walls)
	{
		auto wallBounds = wall->GetGlobalBounds();
		if (wallBounds.intersects(body.getGlobalBounds()))
		{
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->ReturnBullet(this);
			return;
		}
	}

	switch (currentOwner)
	{
	case Owner::Player:
		FixedUpdatePlayer(dt);
		break;
	case Owner::Enemies:
		FixedUpdateEnemies(dt);
		break;
	}
}

void Bullet::FixedUpdatePlayer(float dt)
{
	auto enemies = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetEnemies();
	for (auto enemy : enemies)
	{
		if (enemy->GetCollisionBox().getGlobalBounds().intersects(body.getGlobalBounds()))
		{
			if (!enemy->isDie() && !enemy->isStun())
			{
				enemy->OnHit(weaponStatus, direction);
				dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->ReturnBullet(this);
			}
		}
	}
}

void Bullet::FixedUpdateEnemies(float dt)
{
	auto player = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetPlayer();
	if (player->GetCollisionBox().getGlobalBounds().intersects(body.getGlobalBounds()))
	{
		player->OnHit(weaponStatus, direction);
		if (!player->IsDead())
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->ReturnBullet(this);
	}
}

void Bullet::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Bullet::Fire(sf::Vector2f direction, Player* owner, Weapon::WeaponStatus weaponStatus)
{
	this->direction = direction;
	this->currentOwner = Owner::Player;
	this->weaponStatus = weaponStatus;
	SetPosition(owner->GetPosition());
	SetRotation(Utils::Angle(direction));
}

void Bullet::Fire(sf::Vector2f direction, Enemy* owner, Weapon::WeaponStatus weaponStatus)
{
	this->direction = direction;
	this->currentOwner = Owner::Enemies;
	this->weaponStatus = weaponStatus;
	SetPosition(owner->GetPosition());
	SetRotation(Utils::Angle(direction));
}
