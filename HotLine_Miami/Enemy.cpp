#include "stdafx.h"
#include "Enemy.h"
#include <SceneDev_K.h>
#include "SceneDevS.h"
#include "Player.h"

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
	player = dynamic_cast<SceneDev_K*>(SCENE_MGR.GetCurrentScene())->GetPlayer();
	animatorBody.SetTarget(&body);
	animatorLegs.SetTarget(&legs);
	viewAngle.setPointCount(3);
	viewAngle.setPoint(0, { 0.f,0.f });
	viewAngle.setPoint(1, { 200.f,-30.f });
	viewAngle.setPoint(2, { 200.f,30.f });
	viewAngle.setFillColor(sf::Color::Red);
	SetOrigin(Origins::MC);
	SetStatus(Status::Normal);
	SetPatterns();
	weaponStatus.weaponType = Weapon::WeaponType::Bat;
	hp = 1;
}

void Enemy::SetPatterns()
{
	normal.moveTimer = 0.f;
	normal.movingTimer = 0.f;
	normal.isMoving = false;
	normal.movingCnt = 0;

	idle.lookAwayTimer = 0.f;

<<<<<<< HEAD
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

=======
>>>>>>> origin/Dev_S
	patrol.originPoint.setRadius(0.1f);
	Utils::SetOrigin(patrol.originPoint, Origins::MC);
	patrol.originPoint.setFillColor(sf::Color::Blue);
}

void Enemy::Update(float dt)
{
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

<<<<<<< HEAD
=======
	/*if (Utils::RayCast(position, direction, 300.f, dynamic_cast<Player*>(dynamic_cast<SceneDev_K*>(SCENE_MGR.GetCurrentScene())->GetPlayer())))
		OnHit(1, { -1.f,1.f });*/

>>>>>>> origin/Dev_S
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
	if (viewAngle.getGlobalBounds().intersects(player->GetHitBox().rect.getGlobalBounds()))
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
		SetOrigin(Origins::MC);
		break;
	case Status::Idle:
	{
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
		SetOrigin(Origins::MC);
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
		SetOrigin(Origins::MC);
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
		SetOrigin(Origins::MC);
		break;
	case Status::Stun:
		animatorBody.Play("animations/Enemy/enemy_stun.json");
		SetOrigin(Origins::MC);
		isWalking = false;
		break;
	case Status::GetUp:
		animatorBody.Play("animations/Enemy/enemy_getup.json");
		SetOrigin(Origins::MC);
		isWalking = false;
		break;
	case Status::Die:
		animatorBody.Play("animations/Enemy/enemy_back_bashed.json");
		SetOrigin(Origins::MC);
		isWalking = false;
		break;
	}
}

void Enemy::SetWayPoints(std::vector<sf::Vector2f> pos)
{
	for (auto& vec2f : pos)
	{
		Patrol::WayPoint temp(vec2f);
<<<<<<< HEAD
		temp.point.setRadius(0.1f);
		temp.point.setFillColor(sf::Color::Transparent);
=======
		temp.point.setRadius(0.5f);
>>>>>>> origin/Dev_S
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
	for (auto point : patrol.wayPoints)
		window.draw(point.point);
	window.draw(patrol.originPoint);
	hitBox.Draw(window);
}

void Enemy::PickupWeapon(Weapon* weapon)
{

}

void Enemy::OnHit(Weapon::WeaponStatus weaponStatus, sf::Vector2f direction)
{
	//���� ���

	hp -= weaponStatus.damage;
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
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::Bat:
		animatorBody.Play("animations/Enemy/enemy_bat_attack.json");
		break;
	case Weapon::WeaponType::Knife:
		animatorBody.Play("animations/Enemy/enemy_knife_attack.json");
		break;
	case Weapon::WeaponType::Machinegun:
		animatorBody.Play("animations/Enemy/enemy_m16_attack.json");
		break;
	case Weapon::WeaponType::Shotgun:
		animatorBody.Play("animations/Enemy/enemy_shotgun_attack.json");
		break;
	}
	player->OnHit((int)weaponStatus.weaponType, direction);
}
