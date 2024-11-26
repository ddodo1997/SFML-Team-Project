#include "stdafx.h"
#include "Player.h"

Player::Player(const std::string& name)
	: GameObject(name)
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	leg.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	leg.setScale(scale);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		origin = Utils::SetOrigin(leg, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	leg.setOrigin(origin);
}

void Player::Init()
{
	position = { 50,50 };
	animatorBody.SetTarget(&body);
	animatorLeg.SetTarget(&leg);
	SetOrigin(Origins::MC);

	//aniClipMap.insert("", )
}

void Player::Release()
{

}

void Player::Reset()
{

}

void Player::Update(float dt)
{
	animatorBody.Update(dt);
	animatorLeg.Update(dt);

	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		Utils::Normalize(direction);
	}

	leg.setRotation(Utils::Angle(direction));
	body.setRotation(Utils::Angle(Utils::GetNormal((sf::Vector2f)VIEW_MGR.ScreenToWorld(InputMgr::GetMousePosition())-position)));
	Utils::SetOrigin(leg, Origins::MC);
	Utils::SetOrigin(body, Origins::MC);


	if (mag > 0)
	{
		if(!isMoving)
		animatorLeg.Play("animations/pLegAni.json");
		animatorBody.Play("animations/pBodyAni_default.json");
		isMoving = true;
	}
	else
	{
		animatorBody.Play("animations/pBodyAni_default.json");
		isMoving = false;
	}

	SetPosition(position + direction * speed * dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	if (isMoving)
		window.draw(leg);
	window.draw(body);
}
