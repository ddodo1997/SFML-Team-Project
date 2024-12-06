#include "stdafx.h"
#include "BodyGuard.h"
#include "Player.h"
#include "Weapon.h"
#include "MafiaBoss.h"
#include "Fountain.h"
#include "SceneGame.h"
BodyGuard::BodyGuard(const std::string& name)
	: GameObject(name)
{
}
void BodyGuard::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	legs.setPosition(position);
	collisionBox.setPosition(position);
	meleeHitBox.setPosition(position);
}

void BodyGuard::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	legs.setRotation(rotation);
	collisionBox.setRotation(rotation);
	meleeHitBox.setRotation(rotation);
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
		Utils::SetOrigin(legs, originPreset);
		Utils::SetOrigin(collisionBox, originPreset);
		Utils::SetOrigin(meleeHitBox, Origins::ML);
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

void BodyGuard::SetMafiaBoss(MafiaBoss* boss)
{
	mafiaBoss = boss;
}

void BodyGuard::Init()
{
}

void BodyGuard::Release()
{
}

void BodyGuard::Reset()
{
	animatorBody.SetTarget(&body);
	animatorLegs.SetTarget(&legs);
	animatorBody.Play("animations/Boss2/boss2_bodyguard_idle.json");
	animatorLegs.Play("animations/Boss2/boss2_bodyguard_legs.json");
	meleeHitBox.setSize({ 20.f,30.f });
	meleeHitBox.setFillColor(sf::Color::Transparent);
	meleeHitBox.setOutlineColor(sf::Color::Green);
	meleeHitBox.setOutlineThickness(1.f);
	collisionBox.setSize({ 10.f,10.f });
	collisionBox.setFillColor(sf::Color::Transparent);
	collisionBox.setOutlineColor(sf::Color::Green);
	collisionBox.setOutlineThickness(1.f);
	isWalking = false;
	ResetPatterns();
	SetOrigin(Origins::MC);
}

void BodyGuard::ResetPatterns()
{
	pattern.crawlTimer = 0.f;
	pattern.crawlingTimer = 0.f;
	pattern.poundingTimer = 0.f;

	auto type = Weapon::WeaponType::Knife;
	weaponStatus = WEAPON_TABLE->Get(type);

	pattern.isCrawling = false;
}

void BodyGuard::Update(float dt)
{
	hitBox.UpdateTr(collisionBox, collisionBox.getLocalBounds());

	animatorBody.Update(dt);
	if (isWalking)
		animatorLegs.Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::N))
	{
		SetStatus(Status::Die);
	}

	direction = Utils::GetNormal(player->GetPosition() - position);

	switch (pattern.currentStatus)
	{
	case Status::None:
	case Status::Phase:
		if (player->GetPosition().y < mafiaBoss->GetPosition().y)
			Throw();
		break;
	}
}

void BodyGuard::Phase2(float dt)
{
	if (player->IsDead())
		return;

	switch (pattern.currentStatus)
	{
	case Status::None:
	case Status::Phase:	
		break;
	case Status::Attack:
		OnAttack(dt);
		break;
	case Status::Stun:
		OnStun(dt);
		break;
	case Status::Crawl:
		OnCrawl(dt);
		break;
	case Status::Die:
		OnDie(dt);
		break;
	}
}

void BodyGuard::OnAttack(float dt)
{
	if (meleeHitBox.getGlobalBounds().intersects(player->GetCollisionBox().getGlobalBounds()))
	{
		Attack();
		player->OnHit(weaponStatus, direction);
		if (player->IsDead())
		{
			return;
		}
	}
	SetRotation(Utils::Angle(direction));
	SetPosition(position + direction * pattern.attackSpeed * dt);
}

void BodyGuard::OnStun(float dt)
{
}

void BodyGuard::OnCrawl(float dt)
{
	if (pattern.isCrawling)
	{
		pattern.crawlingTimer += dt;
		if (pattern.crawlingDelay <= pattern.crawlingTimer)
		{
			pattern.crawlingTimer = 0.f;
			pattern.isCrawling = false;
			animatorBody.Stop();
		}
		SetPosition(position + (-direction) * pattern.crawlingSpeed * dt);
		SetRotation(Utils::Angle(-direction));
	}
	else
	{
		pattern.crawlTimer += dt;
		if (pattern.crawlDelay <= pattern.crawlTimer)
		{
			pattern.crawlTimer = 0.f;
			pattern.isCrawling = true;
			animatorBody.RePlay();
		}
	}
}

void BodyGuard::OnDie(float dt)
{
}

void BodyGuard::SetStatus(Status stat)
{
	SetActive(true);
	auto prevStatus = pattern.currentStatus;
	pattern.currentStatus = stat;
	isWalking = false;
	switch (pattern.currentStatus)
	{
	case Status::None:
		SetRotation(90);
		break;
	case Status::Phase:
		break;
	case Status::Throw:
		animatorBody.Play("animations/Boss2/boss2_bodyguard_throw.json");
		SetRotation(Utils::Angle(direction));
		break;
	case Status::Attack:
		isWalking = true;
		break;
	case Status::Stun:
		SetRotation(Utils::Angle(-direction));
		body.setTexture(TEXTURE_MGR.Get("graphics/Enemy/Boss2/boss2_bodyguard_bashed.png"), true);
		animatorBody.Stop();
		break;
	case Status::Pounded1:
		SetActive(false);
		break;
	case Status::Crawl:
		animatorBody.Play("animations/Boss2/boss2_bodyguard_turn.json");
		animatorBody.PlayQueue("animations/Boss2/boss2_bodyguard_crawl.json");
		break;
	case Status::Pounded2:
		SetActive(false);
		break;
	case Status::Die:
		SetRotation(Utils::Angle(-direction));
		body.setTexture(TEXTURE_MGR.Get("graphics/Enemy/Boss2/boss2_bodyguard_dead.png"), true);
		animatorBody.Stop();
		for (int i = 0; i < 3; i++)
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->OnWeaponDrop(weaponStatus, position);
		break;
	}
	SetOrigin(Origins::MC);
}

void BodyGuard::Draw(sf::RenderWindow& window)
{
	if (isWalking)
		window.draw(legs);
	window.draw(body);

	if (Variables::isDrawHitBox)
	{
		window.draw(collisionBox);
		window.draw(meleeHitBox);
	}
}

void BodyGuard::Awaken()
{
	SetStatus(Status::Attack);
}

void BodyGuard::Attack()
{
	isWalking = false;
	animatorBody.Play("animations/Boss2/boss2_bodyguard_attack.json");
	body.setOrigin({25.f,45.f});
}

void BodyGuard::Throw()
{
	dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->OnWeaponThrow(weaponStatus, direction, position,this);
	player->SetControlable(false);
	SetStatus(Status::Throw);
}

void BodyGuard::OnHit()
{
	if (pattern.currentStatus != Status::Attack)
		return;
	SetStatus(Status::Stun);
}

void BodyGuard::OnDie()
{
}
