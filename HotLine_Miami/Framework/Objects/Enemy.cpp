#include "stdafx.h"
#include "Enemy.h"
#include <SceneDev_K.h>
#include "SceneDevS.h"
#include "SceneDevL.h"
#include "Player.h"
#include "Wall2.h"
#include "Wall.h"
#include "Decoration.h"
#include "Bullet.h"
#include "SceneGame.h"

Enemy::Enemy(const std::string& name)
	: GameObject(name)
{

}

void Enemy::SetPosition(const sf::Vector2f& pos)
{
	prevPos = position;
	position = pos;
	body.setPosition(position);
	legs.setPosition(position);
	viewAngle.setPosition(position);
	meleeHitBox.setPosition(position);
	collisionBox.setPosition(position);
}

void Enemy::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	legs.setRotation(rotation);
	viewAngle.setRotation(rotation);
	meleeHitBox.setRotation(rotation);
}


void Enemy::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	legs.setScale(scale);
	viewAngle.setScale(scale);
	meleeHitBox.setScale(scale);
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
	sortingOrder = 0;
}

void Enemy::Release()
{
}

void Enemy::Reset()
{
	// 각자 테스트할 Scene에 맞추어 Scene 추가
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	player = sceneGame->GetPlayer();
	walls = sceneGame->GetWalls();
	decorations = sceneGame->GetDecorations();
	animatorBody.SetTarget(&body);
	animatorLegs.SetTarget(&legs);
	viewAngle.setPointCount(3);
	viewAngle.setPoint(0, { 0.f, 0.f });

	viewAngle.setFillColor(sf::Color(255, 0, 0, 140));

	viewAngle.setPoint(1, { 100.f, -30.f });
	viewAngle.setPoint(2, { 100.f, 30.f });

	collisionBox.setSize({ 10.f,10.f });
	Utils::SetOrigin(collisionBox, Origins::MC);

	weaponSearchRange.setRadius(30.f);
	SetOrigin(Origins::MC);
	SetStatus(Status::Patrol);
	direction = { 0.f, 0.f };
	SetPatterns();
	speed = 30.f;
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

	patrol.currentWayPoint = 0;
	patrol.originPoint.setRadius(0.1f);
	Utils::SetOrigin(patrol.originPoint, Origins::MC);
	patrol.originPoint.setFillColor(sf::Color::Blue);

	stunTimer = 0.f;
}

void Enemy::Update(float dt)
{
	SetOrigin(Origins::MC);
	hitBox.UpdateTr(collisionBox, collisionBox.getLocalBounds());
	animatorBody.Update(dt);
	animatorLegs.Update(dt);

	if (currentStatus == Status::EditorMode)
	{
		return;
	}

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

	if (player->IsDead() || isDie())
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
	case Status::PathFinding :
		UpdatePathFinding(dt);
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
	case Status::StunOnWall:
		UpdateStunOnWall(dt);
		break;
	case Status::GetUp:
		UpdateGetUp(dt);
		break;
	case Status::Die:
		UpdateDie(dt);
		break;
	case Status::Pounded:
		UpdatePounded(dt);
		break;
	}

	if (Utils::Magnitude(direction) > 0.f)
	{
		SetRotation(Utils::Angle(direction));
	}
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
	patrol.currentWayPoint %= patrol.wayPointCnt;
	if (!patrol.originPoint.getGlobalBounds().intersects(patrol.wayPoints[patrol.currentWayPoint].point.getGlobalBounds()))
		direction = Utils::GetNormal(patrol.wayPoints[patrol.currentWayPoint].position - position);
	else
		patrol.currentWayPoint++;
}

void Enemy::UpdatePathFinding(float dt)
{
	patrol.originPoint.setPosition(position);

	if (patrol.currentWayPoint == patrol.wayPointCnt - 1)
		SetStatus(Status::Normal);

	if (!patrol.originPoint.getGlobalBounds().intersects(patrol.wayPoints[patrol.currentWayPoint].point.getGlobalBounds()))
		direction = Utils::GetNormal(patrol.wayPoints[patrol.currentWayPoint].position - position);
	else
		patrol.currentWayPoint++;
}

void Enemy::UpdateAggro(float dt)
{
	direction = Utils::GetNormal(player->GetPosition() - position);
	attackTimer += dt;

	if (attackTimer >= weaponStatus.attackInterval)
	{
		switch (weaponStatus.weaponType)
		{
		case Weapon::WeaponType::Bat:
		case Weapon::WeaponType::Knife:
			speed = 100.f;
			meleeHitBox.setPosition(position);
			if (Utils::Distance(position, player->GetPosition()) < meleeHitBox.getLocalBounds().width)
			{
				attackTimer = 0.f;
				Attack();
			}
			break;
		case Weapon::WeaponType::Machinegun:
		case Weapon::WeaponType::Shotgun:
			attackTimer = 0.f;
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
	auto& activeWeapons = sceneGame->GetActiveWeapons();
	std::map<float, Weapon*> closetWeaponPos;
	Weapon* closetWeapon = nullptr;
	for (auto weapon : activeWeapons)
	{
		closetWeaponPos.insert({ Utils::Distance(position, weapon->GetPosition()), weapon });
		closetWeapon = closetWeaponPos.begin()->second;
	}
	if (closetWeapon == nullptr || Utils::Distance(closetWeapon->GetPosition(), position) > 100.f)
	{
		//루팅할 무기가 없는 경우...
		direction = Utils::GetNormal(player->GetPosition() - position);
		if (Utils::Distance(position, player->GetPosition()) > 80.f)
			speed = 50.f;
		else
			speed = -50.f;
		return;
	}
	speed = 50.f;
	direction = Utils::GetNormal(closetWeapon->GetPosition() - position);

	if (closetWeapon->GetGlobalBounds().intersects(legs.getGlobalBounds()) && closetWeapon->GetIsPickupable())
		PickupWeapon(closetWeapon);
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

void Enemy::UpdateStunOnWall(float dt)
{
	stunTimer += dt;
	if (stunTimer >= stunDelay)
	{
		stunTimer = 0.f;
		SetStatus(Status::GetUp);
	}
}

void Enemy::UpdateGetUp(float dt)
{
	if (animatorBody.GetCurrentClipId() != "EnemyGetUp" && animatorBody.GetCurrentClipId() != "EnemyGetUpOnWall")
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

void Enemy::UpdatePounded(float dt)
{
	speed = 0.f;
}

void Enemy::FixedUpdate(float dt)
{
	if (isDie() || currentStatus == Status::Stun || currentStatus == Status::GetUp || currentStatus == Status::EditorMode || currentStatus == Status::Pounded)
		return;

	if (viewAngle.getGlobalBounds().intersects(player->GetHitBox().rect.getGlobalBounds()) && currentStatus != Status::Aggro && weaponStatus.weaponType != Weapon::WeaponType::None)
		if (!Utils::RayCast(position, Utils::GetNormal(player->GetPosition()- position), viewAngle.getLocalBounds().width, player))
			SetStatus(Status::Aggro);
}

void Enemy::SetStatus(Status stat)
{
	auto prevStatus = currentStatus;
	currentStatus = stat;
	animatorLegs.Play("animations/Enemy/enemy_legs.json");
	isWalking = true;
	stunTimer = 0.f;
	switch (currentStatus)
	{
	case Status::EditorMode:
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
	case Status::PathFinding:
		SetWayPoints(pathFinder.FindPath(position, player->GetPosition()));
		patrol.currentWayPoint = 1;
		speed = 70.f;
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
	case Status::StunOnWall:
		animatorBody.Play("animations/Enemy/enemy_stun_on_wall.json");
		speed = 0.f;
		isWalking = false;
		break;
	case Status::GetUp:
		if (prevStatus == Status::StunOnWall)
			animatorBody.Play("animations/Enemy/enemy_getup_on_wall.json");
		else
			animatorBody.Play("animations/Enemy/enemy_getup.json");
		animatorBody.PlayQueue("animations/Enemy/enemy_none_sneak.json");
		isWalking = false;
		break;
	case Status::Die:
		animatorBody.Play("animations/Enemy/enemy_back_bashed.json");
		isWalking = false;
		break;
	case Status::Pounded:
		animatorBody.Play("animations/Enemy/enemy_stun.json");
		isWalking = false;
		break;
	}
}

void Enemy::SetWeapon(Weapon::WeaponType type)
{
	weaponStatus = WEAPON_TABLE->Get(type);
	if (!weaponStatus.isRangedWeapon)
	{
		meleeHitBox.setSize({ weaponStatus.hitBoxWidth, weaponStatus.hitBoxHeight });
		Utils::SetOrigin(meleeHitBox, Origins::ML);
	}
}

void Enemy::SetWayPoints(std::vector<sf::Vector2f> pos)
{
	clearWayPoints();
	for (auto& vec2f : pos)
	{
		Patrol::WayPoint temp(vec2f);
		temp.point.setRadius(0.1f);
		temp.point.setFillColor(sf::Color::Transparent);
		
		//temp.point.setRadius(3.f);
		//temp.point.setFillColor(sf::Color::Blue);

		patrol.wayPoints.push_back(temp);
	}
	patrol.wayPointCnt = patrol.wayPoints.size();
}

void Enemy::clearWayPoints()
{
	patrol.currentWayPoint = 0;
	patrol.wayPoints.clear();
	patrol.wayPointCnt = patrol.wayPoints.size();
}

void Enemy::Draw(sf::RenderWindow& window)
{
	if (isWalking)
		window.draw(legs);

	if(currentStatus != Status::Pounded)
		window.draw(body);

	for (auto& point : patrol.wayPoints)
		window.draw(point.point);

	window.draw(patrol.originPoint);
	hitBox.Draw(window);

	if (Variables::isDrawHitBox)
	{
		window.draw(viewAngle);
		window.draw(collisionBox);
	}
}

void Enemy::PickupWeapon(Weapon* weapon)
{
	weaponStatus = weapon->GetStatus();
	direction = Utils::GetNormal(player->GetPosition() - position);
	SetStatus(Status::Normal);
	sceneGame->ReturnWeapon(weapon);
}

void Enemy::OnHit(Weapon::WeaponStatus weaponStatus, sf::Vector2f direction, bool isThrow)
{
	isThrow ? hp -= weaponStatus.damageOnThrow : hp -= weaponStatus.damage;
	speed = 150.f;
	DropWeapon();
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
	if (weaponStatus.weaponType == Weapon::WeaponType::None)
		return;
	sceneGame->OnWeaponDrop(weaponStatus, position);
	SetWeapon(Weapon::WeaponType::None);
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
			if (meleeHitBox.getGlobalBounds().intersects(player->GetGlobalBounds()))
				player->OnHit(weaponStatus, direction);
			animatorBody.Play("animations/Enemy/enemy_bat_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_bat_search.json");
			break;
		case Weapon::WeaponType::Knife:
			if (meleeHitBox.getGlobalBounds().intersects(player->GetGlobalBounds()))
				player->OnHit(weaponStatus, direction);
			animatorBody.Play("animations/Enemy/enemy_knife_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_knife_search.json");
			break;
		case Weapon::WeaponType::Machinegun:
			if (weaponStatus.remainingBullet <= 0)
				break;
			sceneGame->SpawnBullet()->Fire(Utils::AngleSpread(direction, 10), this, weaponStatus);
			weaponStatus.remainingBullet--;
			animatorBody.Play("animations/Enemy/enemy_m16_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_m16_search.json");
			break;
		case Weapon::WeaponType::Shotgun:
			if (weaponStatus.remainingBullet <= 0)
				break;
			for (int i = 0; i < 6; i++)
				sceneGame->SpawnBullet()->Fire(Utils::AngleSpread(direction, 10), this, weaponStatus);
			weaponStatus.remainingBullet--;
			animatorBody.Play("animations/Enemy/enemy_shotgun_attack.json");
			animatorBody.PlayQueue("animations/Enemy/enemy_shotgun_search.json");
			break;
		}
	}
}

void Enemy::AddWayPoint(const sf::Vector2f& pos)
{
	patrol.wayPoints.push_back(pos);
}
