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
	collisionBox.setPosition(position);
	meleeHitBox.setPosition(position);
}

void Panther::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	collisionBox.setRotation(rotation);
	meleeHitBox.setRotation(rotation);
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
		Utils::SetOrigin(collisionBox, originPreset);
		Utils::SetOrigin(meleeHitBox, Origins::TC);
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
	meleeHitBox.setSize({ 20.f,15.f });
	meleeHitBox.setFillColor(sf::Color::Transparent);
	meleeHitBox.setOutlineColor(sf::Color::Green);
	meleeHitBox.setOutlineThickness(1.f);
	collisionBox.setSize({ 10.f,10.f });
	collisionBox.setFillColor(sf::Color::Transparent);
	collisionBox.setOutlineColor(sf::Color::Green);
	collisionBox.setOutlineThickness(1.f);	
	SetOrigin(Origins::MC);
	ResetPatterns();
}

void Panther::ResetPatterns()
{
	pattern.aggroTimer = 0.f;
	pattern.attackTimer = 0.f;
	pattern.stunTimer = 0.f;
	pattern.stunStack = 0;
	auto type = Weapon::WeaponType::Knife;
	weaponStatus = WEAPON_TABLE->Get(type);
}

void Panther::Update(float dt)
{
	animator.Update(dt);
	hitBox.UpdateTr(collisionBox, collisionBox.getLocalBounds());
	if (InputMgr::GetKeyDown(sf::Keyboard::M))
	{
		OnHit();
	}
}

void Panther::Phase1(float dt)
{
	direction = Utils::GetNormal(player->GetPosition() - position);

	switch (pattern.currentStatus)
	{
	case Status::Pause:
		OnPause(dt);
		break;
	case Status::Aggro:
		OnAggro(dt);
		break;
	case Status::Attack:
		OnAttack(dt);
		break;
	case Status::Stun:
		OnStun(dt);
		break;
	case Status::Die:
		OnDie(dt);
		break;
	}
}

void Panther::OnPause(float dt)
{
	if (player->IsDead())
	{
		return;
	}
	pattern.aggroTimer += dt;
	if (pattern.aggroDelay <= pattern.aggroTimer)
	{
		pattern.aggroTimer = 0.f;
		SetStatus(Status::Aggro);
	}
}

void Panther::OnAggro(float dt)
{
	if (player->IsDead())
	{
		SetStatus(Status::Pause);
	}
	pattern.attackTimer += dt;
	if (pattern.attackDelay <= pattern.attackTimer)
	{
		pattern.attackTimer = 0.f;
		SetStatus(Status::Attack);
	}
	SetRotation(Utils::Angle(direction) - 90.f);
	SetPosition(position + direction * pattern.speed * dt);
}

void Panther::OnAttack(float dt)
{
	if (meleeHitBox.getGlobalBounds().intersects(player->GetCollisionBox().getGlobalBounds()))
	{
		player->OnHit(weaponStatus, direction); 
		if (player->IsDead())
		{
			SetStatus(Status::HappyMeal);
		}
	}
	SetRotation(Utils::Angle(direction) - 90.f);
	SetPosition(position + direction * pattern.attackSpeed * dt);
}

void Panther::OnStun(float dt)
{
	pattern.stunTimer += dt;
	if (pattern.stunDelay <= pattern.stunTimer)
	{
		pattern.stunTimer = 0.f;
		pattern.stunStack++;
		SetStatus(Status::Attack);
	}
}

void Panther::OnDie(float dt)
{
}

void Panther::SetStatus(Status stat)
{
	auto prevStat = pattern.currentStatus;
	pattern.currentStatus = stat;
	switch (pattern.currentStatus)
	{
	case Status::None:
		SetRotation(0.f);
		break;
	case Status::Pause:
		animator.Play("animations/Boss2/boss2_panther_sit.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Aggro:
		animator.Play("animations/Boss2/boss2_panther_walk.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Attack:
		animator.Play("animations/Boss2/boss2_panther_run.json");
		SetOrigin(Origins::MC);
		break;
	case Status::HappyMeal:
		animator.Play("animations/Boss2/boss2_panther_bite.json");
		SetPosition(player->GetCollisionBox().getPosition());
		SetRotation(Utils::Angle(direction));
		break;
	case Status::Stun:
		animator.Play("animations/Boss2/boss2_panther_hit.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Die:
		animator.Play("animations/Boss2/boss2_panther_die.json");
		SetOrigin(Origins::MC);
		break;
	}
}

void Panther::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	if (Variables::isDrawHitBox)
	{
		window.draw(collisionBox);
		window.draw(meleeHitBox);
	}
}

void Panther::Awaken()
{
	SetStatus(Status::Pause);
}

void Panther::OnHit()
{
	if (pattern.currentStatus != Status::Attack)
		return;
	pattern.stunStack == 0 ? SetStatus(Status::Stun) : SetStatus(Status::Die);
}
