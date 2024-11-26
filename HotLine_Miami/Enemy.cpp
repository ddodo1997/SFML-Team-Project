#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(const std::string& name)
	: GameObject(name)
{
}

void Enemy::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Enemy::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}


void Enemy::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Enemy::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Enemy::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Enemy::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Enemy::Release()
{
}

void Enemy::Reset()
{
	animator.SetTarget(&body);
	SetOrigin(Origins::MC);
	SetStatus(Status::Normal);
}

void Enemy::SetPatterns()
{
	normal.moveTimer = 0.f;
	normal.movingTimer = 0.f;
	normal.isMoving = false;
	normal.movingCnt = 0;

	idle.lookAwayTimer = 0.f;
}

void Enemy::Update(float dt)
{
	animator.Update(dt);
	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		OnHit(1, { -1.f,0.f });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		OnHit(0, { -1.f,0.f });
	}

	switch (currentStatus)
	{
	case Status::Normal:
		UpdateNormal(dt);
		break;
	case Status::Idle:
		UpdateIdle(dt);
		break;
	case Status::Patrol:
		UpdatePatrol(dt);
		break;
	case Status::Aggro:
		UpdateAggro(dt);
		break;
	case Status::SearchWeapon:
		UpdateSearchWeapon(dt);
		break;
	case Status::Stun:
		UpdateStun(dt);
		break;
	case Status::Die:
		UpdateDie(dt);
		break;
	}
	SetRotation(Utils::Angle(direction));
	SetPosition(position + direction * speed * dt);
}

void Enemy::UpdateNormal(float dt)
{

	if (normal.isMoving)
	{
		normal.movingTimer += dt;
		if (normal.movingDelay <= normal.movingTimer)
		{
			normal.movingTimer = 0.f;
			normal.movingCnt--;
			direction = Utils::GetNormal(Utils::RandomOnUnitCircle());
		}
		if (normal.movingCnt == 0)
		{
			normal.isMoving = false;
			direction = { 0.f,0.f };
			animator.Play("animations/Enemy/enemy_none_stand.json");
			SetOrigin(Origins::MC);
		}
	}
	else 
	{
		normal.moveTimer += dt;
		if (normal.moveDelay <= normal.moveTimer)
		{
			normal.moveTimer = 0.f;
			normal.isMoving = true;
			normal.movingCnt = Utils::RandomRange(3, 7);
			normal.movingDelay = Utils::RandomRange(1.f, 3.f);
			direction = Utils::GetNormal(Utils::RandomOnUnitCircle());
			animator.Play("animations/Enemy/enemy_none_walk.json");
			SetOrigin(Origins::MC);
		}
	}
}

void Enemy::UpdateIdle(float dt)
{
	idle.lookAwayTimer += dt;
	if (idle.lookAwayDelay <= idle.lookAwayTimer)
	{
		idle.lookAwayTimer = 0.f;
		direction = Utils::GetNormal(Utils::RandomOnUnitCircle());
	}
}

void Enemy::UpdatePatrol(float dt)
{
}

void Enemy::UpdateAggro(float dt)
{
	//TODO : 길찾기 알고리즘. 설현기 나와!!
	//TODO : 레이캐스트. 김승욱 나와!!
}

void Enemy::UpdateSearchWeapon(float dt)
{
	//주위 반경 어느정도에서 무기를 찾고, 찾았다면 무기를 주우러가며 찾지 못했다면 플레이어와 일정 거리를 유지하려 함
}

void Enemy::UpdateStun(float dt)
{
	if (speed >= 0.f)
	{
		speed -= dt * 300;
	}
	stunTimer += dt;
	if (stunTimer >= stunDelay)
	{
		stunTimer = 0.f;
		SetStatus(Status::GetUp);
	}
}

void Enemy::UpdateGetUp(float dt)
{
	if (animator.GetCurrentClipId() != "EnemyGetUp")
		SetStatus(Status::SearchWeapon);
}

void Enemy::UpdateDie(float dt)
{
	if (speed >= 0.f)
		speed -= dt * 300;
}

void Enemy::SetStatus(Status stat)
{
	auto prevStatus = currentStatus;
	currentStatus = stat;

	switch (currentStatus)
	{
	case Status::Normal:
		animator.Play("animations/Enemy/enemy_none_stand.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Idle:
	{
		animator.Play("animations/Enemy/enemy_none_stand.json");
		SetOrigin(Origins::MC);
		idle.lookAwayDelay = Utils::RandomRange(2.f, 5.f);
		speed = 0.f;
		break;
	}
	case Status::Patrol:
		break;
	case Status::Aggro:
		break;
	case Status::SearchWeapon:
		animator.Play("animations/Enemy/enemy_none_walk.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Stun:
		animator.Play("animations/Enemy/enemy_stun.json");
		SetOrigin(Origins::MC);
		break;
	case Status::GetUp:
		animator.Play("animations/Enemy/enemy_getup.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Die:
		animator.Play("animations/Enemy/enemy_back_bashed.json");
		SetOrigin(Origins::MC);
		break;
	}
}

void Enemy::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Enemy::OnHit(int damage, sf::Vector2f direction)
{
	//웨펀 드랍

	hp -= damage;
	if (hp <= 0)
	{
		OnDie(direction);
		return;
	}
	SetStatus(Status::Stun);
	this->direction = Utils::GetNormal(direction);
	SetRotation(Utils::Angle(this->direction));
	speed = 150.f;
}

void Enemy::OnDie(sf::Vector2f direction)
{
	SetStatus(Status::Die);
	this->direction = Utils::GetNormal(direction);
	SetRotation(Utils::Angle(this->direction));
	speed = 150.f;
}