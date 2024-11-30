#include "stdafx.h"
#include "Enemy.h"
#include <SceneDev_K.h>
#include "SceneDevS.h"
#include "SceneDevL.h"
#include "Player.h"
#include "Bullet.h"


Enemy::Enemy(const std::string& name)
	: GameObject(name)
{

}

void Enemy::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	legs.setPosition(position);
	viewAngle.setPosition(position);
}

void Enemy::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	legs.setRotation(rotation);
	viewAngle.setRotation(rotation);
}


void Enemy::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	legs.setScale(scale);
	viewAngle.setScale(scale);
}

void Enemy::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		Utils::SetOrigin(legs, originPreset);
	}
}

void Enemy::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	legs.setOrigin(origin);
}

void Enemy::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 3;
}

void Enemy::Release()
{
}

void Enemy::Reset()
{
	// 각자 테스트할 Scene에 맞추어 Scene 추가
	sceneGame = dynamic_cast<SceneDevL*>(SCENE_MGR.GetCurrentScene());

	player = sceneGame->GetPlayer();
	animatorBody.SetTarget(&body);
	animatorLegs.SetTarget(&legs);
	viewAngle.setPointCount(3);
	viewAngle.setPoint(0, { 0.f,0.f });
	viewAngle.setPoint(1, { 50.f,-10.f });
	viewAngle.setPoint(2, { 50.f,10.f });
	viewAngle.setFillColor(sf::Color::Red);
	SetOrigin(Origins::MC);
	SetWeapon(Weapon::WeaponType::Shotgun);
	SetStatus(Status::Patrol);
	SetPatterns();
	attackTimer = weaponStatus.attackInterval;
	hp = 1;
}

void Enemy::SetPatterns()
{
	normal.moveTimer = 0.f;
	normal.movingTimer = 0.f;
	normal.isMoving = false;
	normal.movingCnt = 0;

	idle.lookAwayTimer = 0.f;

	Patrol::WayPoint waypo(sf::Vector2f(-50.f, -50.f));
	waypo.point.setRadius(0.5f);
	waypo.point.setFillColor(sf::Color::Green);
	patrol.wayPointCnt = 4;
	patrol.wayPoints.push_back(waypo);

	waypo.position = { 50.f,-50.f };
	waypo.point.setPosition(waypo.position);
	patrol.wayPoints.push_back(waypo);

	waypo.position = { 50.f,50.f };
	waypo.point.setPosition(waypo.position);
	patrol.wayPoints.push_back(waypo);

	waypo.position = { -50.f,50.f };
	waypo.point.setPosition(waypo.position);
	patrol.wayPoints.push_back(waypo);

	patrol.originPoint.setRadius(0.1f);
	Utils::SetOrigin(patrol.originPoint, Origins::MC);
	patrol.originPoint.setFillColor(sf::Color::Blue);
}

void Enemy::Update(float dt)
{
	SetOrigin(Origins::MC);
	hitBox.UpdateTr(legs, legs.getLocalBounds());
	animatorBody.Update(dt);
	animatorLegs.Update(dt);
	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		OnHit(weaponStatus, { -1.f,0.f });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		OnHit(weaponStatus, { -1.f,0.f });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		Reset();
	}
	//�þ߰��� �÷��̾�� �浹�� �˻��� ���̸� ����ĳ��Ʈ ����.

	if (player->IsDead())
	{
		animatorLegs.Stop();
		return;
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
	case Status::GetUp:
		UpdateGetUp(dt);
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
			switch (weaponStatus.weaponType)
			{
			case Weapon::WeaponType::None:
				animatorBody.Play("animations/Enemy/enemy_none_stand.json");
				break;
			case Weapon::WeaponType::Bat:
				animatorBody.Play("animations/Enemy/enemy_bat_search.json");
				break;
			case Weapon::WeaponType::Knife:
				animatorBody.Play("animations/Enemy/enemy_knife_search.json");
				break;
			case Weapon::WeaponType::Machinegun:
				animatorBody.Play("animations/Enemy/enemy_m16_search.json");
				break;
			case Weapon::WeaponType::Shotgun:
				animatorBody.Play("animations/Enemy/enemy_shotgun_search.json");
				break;
			}
			animatorLegs.Stop();
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
			switch (weaponStatus.weaponType)
			{
			case Weapon::WeaponType::None:
				animatorBody.Play("animations/Enemy/enemy_none_walk.json");
				break;
			case Weapon::WeaponType::Bat:
				animatorBody.Play("animations/Enemy/enemy_bat_walk.json");
				break;
			case Weapon::WeaponType::Knife:
				animatorBody.Play("animations/Enemy/enemy_knife_walk.json");
				break;
			case Weapon::WeaponType::Machinegun:
				animatorBody.Play("animations/Enemy/enemy_m16_walk.json");
				break;
			case Weapon::WeaponType::Shotgun:
				animatorBody.Play("animations/Enemy/enemy_shotgun_walk.json");
				break;
			}
			animatorLegs.RePlay();
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
	patrol.originPoint.setPosition(position);
	static int wayPointCnt = 1;
	wayPointCnt %= patrol.wayPointCnt;
	if (!patrol.originPoint.getGlobalBounds().intersects(patrol.wayPoints[wayPointCnt].point.getGlobalBounds()))
		direction = Utils::GetNormal(patrol.wayPoints[wayPointCnt].position - position);
	else
		wayPointCnt++;
}

void Enemy::UpdateAggro(float dt)
{
	speed = 100.f;
	direction = Utils::GetNormal(player->GetPosition() - position);
	attackTimer += dt;

	if (attackTimer >= weaponStatus.attackInterval)
	{
		attackTimer = 0.f;
		switch (weaponStatus.weaponType)
		{
		case Weapon::WeaponType::Bat:
		case Weapon::WeaponType::Knife:
			if (Utils::Distance(position, player->GetPosition()) < 10.f)
				Attack();
			break;
		case Weapon::WeaponType::Machinegun:
		case Weapon::WeaponType::Shotgun:
			speed = 0.f;
			Attack();
			break;
		}
	}
}

void Enemy::UpdateSearchWeapon(float dt)
{
	// TODO : Searches for weapons within a certain radius.
	// If not found, moves away from the player.
}

void Enemy::UpdateStun(float dt)
{
	if (speed > 0.f)
	{
		speed -= dt * 300;
	}
	else
	{
		speed = 0.f;
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
	if (animatorBody.GetCurrentClipId() != "EnemyGetUp")
		SetStatus(Status::SearchWeapon);
}

void Enemy::UpdateDie(float dt)
{
	if (speed <= 0.f)
	{
		speed = 0.f;
		return;
	}
	speed -= dt * 300;
}

void Enemy::FixedUpdate(float dt)
{
	if (viewAngle.getGlobalBounds().intersects(player->GetHitBox().rect.getGlobalBounds()) && currentStatus != Status::Aggro)
		SetStatus(Status::Aggro);
}

void Enemy::SetStatus(Status stat)
{
	auto prevStatus = currentStatus;
	currentStatus = stat;
	animatorLegs.Play("animations/Enemy/enemy_legs.json");
	isWalking = true;

	switch (currentStatus)
	{
	case Status::Normal:
		switch (weaponStatus.weaponType)
		{
		case Weapon::WeaponType::None:
			animatorBody.Play("animations/Enemy/enemy_none_walk.json");
			break;
		case Weapon::WeaponType::Bat:
			animatorBody.Play("animations/Enemy/enemy_bat_walk.json");
			break;
		case Weapon::WeaponType::Knife:
			animatorBody.Play("animations/Enemy/enemy_knife_walk.json");
			break;
		case Weapon::WeaponType::Machinegun:
			animatorBody.Play("animations/Enemy/enemy_m16_walk.json");
			break;
		case Weapon::WeaponType::Shotgun:
			animatorBody.Play("animations/Enemy/enemy_shotgun_walk.json");
			break;
		}
		animatorLegs.Stop();
		break;
	case Status::Idle:
	{
		switch (weaponStatus.weaponType)
		{
		case Weapon::WeaponType::None:
			animatorBody.Play("animations/Enemy/enemy_none_stand.json");
			break;
		case Weapon::WeaponType::Bat:
			animatorBody.Play("animations/Enemy/enemy_bat_search.json");
			break;
		case Weapon::WeaponType::Knife:
			animatorBody.Play("animations/Enemy/enemy_knife_search.json");
			break;
		case Weapon::WeaponType::Machinegun:
			animatorBody.Play("animations/Enemy/enemy_m16_search.json");
			break;
		case Weapon::WeaponType::Shotgun:
			animatorBody.Play("animations/Enemy/enemy_shotgun_search.json");
			break;
		}
		animatorLegs.Stop();
		idle.lookAwayDelay = Utils::RandomRange(2.f, 5.f);
		speed = 0.f;
		break;
	}
	case Status::Patrol:
		switch (weaponStatus.weaponType)
		{
		case Weapon::WeaponType::None:
			animatorBody.Play("animations/Enemy/enemy_none_walk.json");
			break;
		case Weapon::WeaponType::Bat:
			animatorBody.Play("animations/Enemy/enemy_bat_walk.json");
			break;
		case Weapon::WeaponType::Knife:
			animatorBody.Play("animations/Enemy/enemy_knife_walk.json");
			break;
		case Weapon::WeaponType::Machinegun:
			animatorBody.Play("animations/Enemy/enemy_m16_walk.json");
			break;
		case Weapon::WeaponType::Shotgun:
			animatorBody.Play("animations/Enemy/enemy_shotgun_walk.json");
			break;
		}
		break;
	case Status::Aggro:
		switch (weaponStatus.weaponType)
		{
		case Weapon::WeaponType::None:
			animatorBody.Play("animations/Enemy/enemy_none_walk.json");
			break;
		case Weapon::WeaponType::Bat:
			animatorBody.Play("animations/Enemy/enemy_bat_walk.json");
			break;
		case Weapon::WeaponType::Knife:
			animatorBody.Play("animations/Enemy/enemy_knife_walk.json");
			break;
		case Weapon::WeaponType::Machinegun:
			animatorBody.Play("animations/Enemy/enemy_m16_walk.json");
			break;
		case Weapon::WeaponType::Shotgun:
			animatorBody.Play("animations/Enemy/enemy_shotgun_walk.json");
			break;
		}
		break;
	case Status::SearchWeapon:
		animatorBody.Play("animations/Enemy/enemy_none_walk.json");
		break;
	case Status::Stun:
		animatorBody.Play("animations/Enemy/enemy_stun.json");
		isWalking = false;
		break;
	case Status::GetUp:
		animatorBody.Play("animations/Enemy/enemy_getup.json");
		isWalking = false;
		break;
	case Status::Die:
		animatorBody.Play("animations/Enemy/enemy_back_bashed.json");
		isWalking = false;
		break;
	}
}

void Enemy::SetWeapon(Weapon::WeaponType type)
{
	weaponStatus = WEAPON_TABLE->Get(type);
}

void Enemy::SetWayPoints(std::vector<sf::Vector2f> pos)
{
	for (auto& vec2f : pos)
	{
		Patrol::WayPoint temp(vec2f);
		temp.point.setRadius(0.1f);
		temp.point.setFillColor(sf::Color::Transparent);
		patrol.wayPoints.push_back(temp);
	}
	patrol.wayPointCnt = patrol.wayPoints.size();
}

void Enemy::clearWayPoints()
{
	patrol.wayPoints.clear();
	patrol.wayPointCnt = patrol.wayPoints.size();
}

void Enemy::Draw(sf::RenderWindow& window)
{
	if (isWalking)
		window.draw(legs);
	window.draw(body);
	for (auto& point : patrol.wayPoints)
		window.draw(point.point);
	window.draw(patrol.originPoint);
	hitBox.Draw(window);
	//window.draw(viewAngle);
}

void Enemy::PickupWeapon(Weapon* weapon)
{
	weaponStatus = weapon->GetStatus();
}

void Enemy::OnHit(Weapon::WeaponStatus weaponStatus, sf::Vector2f direction)
{
	hp -= weaponStatus.damage;
	speed = 150.f;
	if (hp <= 0)
	{
		OnDie(direction);
		return;
	}
	SetStatus(Status::Stun);
	this->direction = Utils::GetNormal(direction);
	SetRotation(Utils::Angle(this->direction));
}

void Enemy::DropWeapon()
{

}

void Enemy::OnDie(sf::Vector2f direction)
{
	SetStatus(Status::Die);
	this->direction = Utils::GetNormal(direction);
	SetRotation(Utils::Angle(this->direction));
	speed = 150.f;
}

void Enemy::Attack()
{
	if (!player->IsDead())
	{
		switch (weaponStatus.weaponType)
		{
		case Weapon::WeaponType::Bat:
			animatorBody.Play("animations/Enemy/enemy_bat_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_bat_search.json");
			break;
		case Weapon::WeaponType::Knife:
			animatorBody.Play("animations/Enemy/enemy_knife_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_knife_search.json");
			break;
		case Weapon::WeaponType::Machinegun:
			sceneGame->SpawnBullet()->Fire(Utils::AngleSpread(direction, 10), this, weaponStatus);
			animatorBody.Play("animations/Enemy/enemy_m16_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_m16_search.json");
			break;
		case Weapon::WeaponType::Shotgun:
			for(int i = 0 ; i < 6 ; i++)
				sceneGame->SpawnBullet()->Fire(Utils::AngleSpread(direction,10), this, weaponStatus);
			animatorBody.Play("animations/Enemy/enemy_shotgun_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_shotgun_search.json");
			break;
		}
	}
}
