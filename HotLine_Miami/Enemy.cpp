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
}

void Enemy::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	legs.setRotation(rotation);
}


void Enemy::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	legs.setScale(scale);
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
	animatorBody.SetTarget(&body);
	animatorLegs.SetTarget(&legs);
	SetOrigin(Origins::MC);
	SetPatterns();
	hp = 1;
}

void Enemy::SetPatterns()
{
	normal.moveTimer = 0.f;
	normal.movingTimer = 0.f;
	normal.isMoving = false;
	normal.movingCnt = 0;

	idle.lookAwayTimer = 0.f;

	Patrol::WayPoint waypo(sf::Vector2f(-50.f,-50.f));
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
	hitBox.UpdateTr(body, body.getLocalBounds());
	animatorBody.Update(dt);
	animatorLegs.Update(dt);
	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		OnHit(1, { -1.f,0.f });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		OnHit(0, { -1.f,0.f });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		SCENE_MGR.ChangeScene(SceneIds::Dev_K);
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

	if (Utils::RayCast(position, direction, 300.f, dynamic_cast<Player*>(dynamic_cast<SceneDev_K*>(SCENE_MGR.GetCurrentScene())->GetPlayer())))
		OnHit(1, { -1.f,1.f });

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
			animatorBody.Play("animations/Enemy/enemy_none_stand.json");
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
			animatorBody.Play("animations/Enemy/enemy_none_walk.json");
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
	static int wayPointCnt = 0;
	wayPointCnt %= patrol.wayPointCnt;
	if (!patrol.originPoint.getGlobalBounds().intersects(patrol.wayPoints[wayPointCnt].point.getGlobalBounds()))
		direction = Utils::GetNormal(patrol.wayPoints[wayPointCnt].position - position);
	else
		wayPointCnt++;
}

void Enemy::UpdateAggro(float dt)
{
	//TODO : ��ã�� �˰�����. ������ �ҷ�!!
}

void Enemy::UpdateSearchWeapon(float dt)
{
	//���� �ݰ� ����������� ���⸦ ã��, ã�Ҵٸ� ���⸦ �ֿ췯���� ã�� ���ߴٸ� �÷��̾�� ���� �Ÿ��� �����Ϸ� ��
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

void Enemy::SetStatus(Status stat)
{
	auto prevStatus = currentStatus;
	currentStatus = stat;
	animatorLegs.Play("animations/Enemy/enemy_legs.json");
	legs.setColor(sf::Color::White);

	switch (currentStatus)
	{
	case Status::Normal:
		animatorBody.Play("animations/Enemy/enemy_none_stand.json"); 
		animatorLegs.Stop();
		SetOrigin(Origins::MC);
		break;
	case Status::Idle:
	{
		animatorBody.Play("animations/Enemy/enemy_none_stand.json");
		animatorLegs.Stop();
		SetOrigin(Origins::MC);
		idle.lookAwayDelay = Utils::RandomRange(2.f, 5.f);
		speed = 0.f;
		break;
	}
	case Status::Patrol:
		animatorBody.Play("animations/Enemy/enemy_none_walk.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Aggro:
		break;
	case Status::SearchWeapon:
		animatorBody.Play("animations/Enemy/enemy_none_walk.json");
		SetOrigin(Origins::MC);
		break;
	case Status::Stun:
		animatorBody.Play("animations/Enemy/enemy_stun.json");
		legs.setColor(sf::Color::Transparent);
		SetOrigin(Origins::MC);
		break;
	case Status::GetUp:
		animatorBody.Play("animations/Enemy/enemy_getup.json");
		legs.setColor(sf::Color::Transparent);
		SetOrigin(Origins::MC);
		break;
	case Status::Die:
		animatorBody.Play("animations/Enemy/enemy_back_bashed.json");
		legs.setColor(sf::Color::Transparent);
		SetOrigin(Origins::MC);
		break;
	}
}

void Enemy::SetWayPoints(std::vector<sf::Vector2f> pos)
{
	for (auto& vec2f : pos)
	{
		Patrol::WayPoint temp(vec2f);
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
	window.draw(legs);
	window.draw(body);
	for (auto point : patrol.wayPoints)
		window.draw(point.point);
	window.draw(patrol.originPoint);
}

void Enemy::OnHit(int damage, sf::Vector2f direction)
{
	//���� ���

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
