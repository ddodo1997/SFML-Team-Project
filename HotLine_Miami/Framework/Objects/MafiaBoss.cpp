#include "stdafx.h"
#include "MafiaBoss.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
MafiaBoss::MafiaBoss(const std::string& name)
	: GameObject(name)
{
}

void MafiaBoss::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	legs.setPosition(position);
	collisionBox.setPosition(position);
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
		Utils::SetOrigin(collisionBox, originPreset);
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
	legs.setTexture(TEXTURE_MGR.Get("graphics/Enemy/Boss2/boss2_legs.png"));
	collisionBox.setSize({ 10.f,10.f });
	collisionBox.setFillColor(sf::Color::Transparent);
	collisionBox.setOutlineColor(sf::Color::Green);
	collisionBox.setOutlineThickness(1.f);
	ResetPatterns();
	SetStatus(Status::None);
	SetOrigin(Origins::MC);
}

void MafiaBoss::ResetPatterns()
{
	pattern.isShanks = false;

	auto type = Weapon::WeaponType::Machinegun;
	weaponStatus = WEAPON_TABLE->Get(type);

	pattern.currentAmmo = pattern.doubleMaxAmmo;
	pattern.doubleReloadingTimer = 0.f;
	pattern.firstHitTimer = 0.f;
	pattern.getGunsTimer = 0.f;
	pattern.soloReloadingTimer = 0.f;
}

void MafiaBoss::Update(float dt)
{
	animatorBody.Update(dt);

}

void MafiaBoss::Phase3(float dt)
{
	if (IsDead())
		return;
	if (player->IsDead())
	{
		SetStatus(Status::None);
		return;
	}
	pattern.targetDirection = Utils::GetNormal(player->GetPosition() - position);
	
	switch (pattern.currentStatus)
	{
	case Status::Awaken:
		Awakning(dt);
		break;
	case Status::DoubleShot:
		DoubleShot(dt);
		break;
	case Status::DoubleReload:
		DoubleReload(dt);
		break;
	case Status::Hit:
		LostArms(dt);
		break;
	case Status::SoloShot:
		SoloShot(dt);
		break;
	case Status::SoloReload:
		SoloReload(dt);
		break;
	case Status::Suicide:
		Suiciding(dt);
		break;
	}
}

void MafiaBoss::Awakning(float dt)
{
	pattern.getGunsTimer += dt;
	if (pattern.getGunsDelay <= pattern.getGunsTimer)
	{
		pattern.getGunsTimer = 0;
		SetStatus(Status::DoubleShot);
	}
}

void MafiaBoss::DoubleShot(float dt)
{
	if (0 >= pattern.currentAmmo)
		SetStatus(Status::DoubleReload);

	pattern.shotTimer += dt;
	if (pattern.shotTimer >= weaponStatus.attackInterval / 2)
	{
		dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->SpawnBullet()->Fire(Utils::AngleSpread(pattern.targetDirection, 10.f), this, weaponStatus);
		pattern.shotTimer = 0.f;
		pattern.currentAmmo--;
	}
	SetRotation(Utils::Angle(pattern.targetDirection) - 90);
}

void MafiaBoss::DoubleReload(float dt)
{
	pattern.doubleReloadingTimer += dt;
	if (pattern.doubleReloadingDelay <= pattern.doubleReloadingTimer)
	{
		pattern.currentAmmo = pattern.doubleMaxAmmo;
		pattern.doubleReloadingTimer = 0.f;
		SetStatus(Status::DoubleShot);
	}
}

void MafiaBoss::LostArms(float dt)
{
	pattern.firstHitTimer += dt;
	if (pattern.firstHitDelay <= pattern.firstHitTimer)
	{
		pattern.firstHitTimer = 0.f;
		pattern.currentAmmo = pattern.soloMaxAmmo;
		pattern.isShanks ? SetStatus(Status::Suicide) : SetStatus(Status::SoloShot);
		pattern.isShanks = !pattern.isShanks;
	}
}

void MafiaBoss::SoloShot(float dt)
{
	if (0 >= pattern.currentAmmo)
		SetStatus(Status::SoloReload);

	pattern.shotTimer += dt;
	if (pattern.shotTimer >= weaponStatus.attackInterval / 4)
	{
		dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->SpawnBullet()->Fire(Utils::AngleSpread(pattern.targetDirection, 10.f), this, weaponStatus);
		pattern.shotTimer = 0.f;
		pattern.currentAmmo--;
	}
	SetRotation(Utils::Angle(pattern.targetDirection) - 90);
}

void MafiaBoss::SoloReload(float dt)
{
	pattern.soloReloadingTimer += dt;
	if (pattern.soloReloadingDelay <= pattern.soloReloadingTimer)
	{
		pattern.currentAmmo = pattern.soloMaxAmmo;
		pattern.soloReloadingTimer = 0.f;
		SetStatus(Status::SoloShot);
	}
}

void MafiaBoss::Suiciding(float dt)
{
	if (animatorBody.IsEnd())
		SetStatus(Status::Die);
	SetRotation(0);
}

void MafiaBoss::Draw(sf::RenderWindow& window)
{
	window.draw(legs);
	window.draw(body);
}

void MafiaBoss::SetStatus(Status stat)
{
	auto prevStat = stat;
	pattern.currentStatus = stat;

	switch (pattern.currentStatus)
	{
	case Status::None:
		animatorBody.Play("animations/Boss2/boss2_idle.json");
		body.setOrigin({ 13.f,16.f });
		break;
	case Status::Awaken:
		animatorBody.Play("animations/Boss2/boss2_get_guns.json");
		body.setOrigin({ 14.f,6.f });
		break;
	case Status::DoubleShot:
		animatorBody.Play("animations/Boss2/boss2_fire_2guns.json");
		body.setOrigin({ 20.f,10.f });
		break;
	case Status::DoubleReload:
		animatorBody.Play("animations/Boss2/boss2_reload_2guns.json");
		body.setOrigin({ 20.f,16.f });
		break;
	case Status::Hit:
		pattern.isShanks ? animatorBody.Play("animations/Boss2/boss2_hit2.json") : animatorBody.Play("animations/Boss2/boss2_hit1.json");
		body.setOrigin({ 22.f,16.f });
		break;
	case Status::SoloShot:
		animatorBody.Play("animations/Boss2/boss2_fire_1gun.json");
		body.setOrigin({ 22.f,16.f });
		break;
	case Status::SoloReload:
		animatorBody.Play("animations/Boss2/boss2_reload_1gun.json");
		body.setOrigin({ 22.f,16.f });
		break;
	case Status::Suicide:
		animatorBody.Play("animations/Boss2/boss2_suicide.json");
		body.setOrigin({ 22.f,16.f });
		break;
	case Status::Die:
		break;
	}
}

void MafiaBoss::Awaken()
{
	SetStatus(Status::Awaken);
}

void MafiaBoss::OnHit()
{
	if(IsHitAble())
		SetStatus(Status::Hit);
}

bool MafiaBoss::IsHitAble() const
{
	bool result = false;
	for (int i = 1; i < 6; i++)
	{
		result = pattern.currentStatus == (Status)i ? true : false;
		if(result)
			break;
	}
	return result;
}
