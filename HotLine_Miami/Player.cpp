#include "stdafx.h"
#include "Player.h"
#include "SceneDevL.h"

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

void Player::SetScene(SceneDevL* sceneDevL)
{
	this->sceneDevL = sceneDevL;
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;

	//aniClipMap.insert("", )
}

void Player::Release()
{

}

void Player::Reset()
{
	isAlive = true;
	speed = 100.f;
	onDieSpeed = 100.f;
	onDieEffectAccumTime = 0.6f;
	position = { 50,50 };
	SetScale({ 1.f, 1.f });
	animatorBody.SetTarget(&body);
	animatorLeg.SetTarget(&leg);
	SetOrigin(Origins::MC);

	SetWeaponStatus();
	attackHitBoxCheck.setFillColor(sf::Color::Transparent);
	attackHitBoxCheck.setOutlineColor(sf::Color::Green);
	attackHitBoxCheck.setOutlineThickness(1.f);
	Utils::SetOrigin(attackHitBoxCheck, Origins::ML);
}

void Player::Update(float dt)
{
	attackTimer += dt;

	hitBox.UpdateTr(body, body.getLocalBounds());
	attackHitBoxCheck.setPosition(body.getPosition());
	attackHitBoxCheck.setRotation(body.getRotation());
	Utils::SetOrigin(attackHitBoxCheck, Origins::ML);

	if (!isAlive)
	{
		isMoving = false;
		UpdateOnDie(dt);
		return;
	}

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

	auto mPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());
	look = mPos - position;
	body.setRotation(Utils::Angle(look));

	Utils::SetOrigin(leg, Origins::MC);
	Utils::SetOrigin(body, Origins::MC);


	if (mag > 0)
	{
		if (!isMoving)
		{
			animatorLeg.Play("animations/Player/pLegAni.json");
			animatorLeg.SetSpeed(-1.f);
			animatorBody.Play("animations/Player/pBodyAni_default.json");
			isMoving = true;
		}
	}
	else
	{
		animatorBody.Play("animations/Player/pBodyAni_default.json");
		isMoving = false;
	}


	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (attackTimer > weaponStatus.attackInterval)
		{
			Attack();
			attackTimer = 0;
		}
	}
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
	{
		if (sceneDevL == nullptr)
			SetScene((SceneDevL*)SCENE_MGR.GetCurrentScene());
		ThrowWeapon(look);
		weaponStatus.weaponType = Weapon::WeaponType::None;
		sceneDevL->PlayerTryPickUpWeapon();
	}

	SetPosition(position + direction * speed * dt);
	hitBox.UpdateTr(body, body.getLocalBounds());
}

void Player::UpdateOnDie(float dt)
{
	if(onDieEffectAccumTime > 0)
		onDieEffectAccumTime -= dt;
	onDieEffectAccumTime = Utils::Clamp(onDieEffectAccumTime, 0.f, 3.f);
	
	SetPosition(position + direction * onDieSpeed * onDieEffectAccumTime * dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	if (!isAlive)
	{

	}
	else if (isMoving)
		window.draw(leg);
	window.draw(body);
	hitBox.Draw(window);
	if (Variables::isDrawHitBox)
	{
		window.draw(attackHitBoxCheck);
	}
}

void Player::OnHit(int weaponType, sf::Vector2f hitDir)
{
	auto normDir = Utils::GetNormal(hitDir);
	switch (weaponType)
	{
	case 0:
		OnHitByBat(normDir);
		break;
	case 1:
		OnHitByKnife(normDir);
		break;
	case 2:
		OnHitByMachinegun(normDir);
		break;
	case 3:
		OnHitByShotgun(normDir);
		break;
	}
	isAlive = false;
	if (weaponStatus.weaponType != Weapon::WeaponType::None)
		DropWeapon(normDir);
}

void Player::OnHitByBat(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(0, 4);
	tempTexId = "graphics/player/Die/sprPBackBlunt2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::OnHitByKnife(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(0, 2);
	tempTexId = "graphics/player/Die/sprPBackCut2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::OnHitByMachinegun(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(1, 4);
	tempTexId = "graphics/player/Die/sprPBackMachinegun2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::OnHitByShotgun(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(1, 5);
	tempTexId = "graphics/player/Die/sprPBackShotgun2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::WeaponPickUp(Weapon::WeaponType weaponType, int remainingBullet)
{
	weaponStatus.weaponType = weaponType;
	SetWeaponStatus();
	if (weaponStatus.isRangedWeapon)
		SetRemainingBullet(remainingBullet);
	else
		SetRemainingBullet(0);
}

void Player::SetWeaponStatus()
{
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		weaponStatus.damage = 0;
		weaponStatus.damageOnThrow = 0;
		weaponStatus.isRangedWeapon = false;
		weaponStatus.maxBullet = 0;
		weaponStatus.attackInterval = 0.5f;
		attackHitBox = { 0.f, 0.f, 25.f, 30.f };
		attackHitBoxCheck.setSize(attackHitBox.getSize());
		break;
	case Weapon::WeaponType::Bat:
		weaponStatus.damage = 1;
		weaponStatus.damageOnThrow = 0;
		weaponStatus.isRangedWeapon = false;
		weaponStatus.maxBullet = 0;
		weaponStatus.attackInterval = 0.5f;
		attackHitBox = { 0.f, 0.f, 40.f, 40.f };
		attackHitBoxCheck.setSize(attackHitBox.getSize());
		break;
	case Weapon::WeaponType::Knife:
		weaponStatus.damage = 1;
		weaponStatus.damageOnThrow = 1;
		weaponStatus.isRangedWeapon = false;
		weaponStatus.maxBullet = 0;
		weaponStatus.attackInterval = 0.5f;
		attackHitBox = { 0.f, 0.f, 30.f, 40.f };
		attackHitBoxCheck.setSize(attackHitBox.getSize());
		break;
	case Weapon::WeaponType::Machinegun:
		weaponStatus.damage = 1;
		weaponStatus.damageOnThrow = 0;
		weaponStatus.isRangedWeapon = true;
		weaponStatus.maxBullet = 24;
		weaponStatus.attackInterval = 0.1f;
		attackHitBox = { 0.f, 0.f, 0.f, 0.f };
		attackHitBoxCheck.setSize(attackHitBox.getSize());
		break;
	case Weapon::WeaponType::Shotgun:
		weaponStatus.damage = 1;
		weaponStatus.damageOnThrow = 0;
		weaponStatus.isRangedWeapon = true;
		weaponStatus.maxBullet = 2;
		weaponStatus.attackInterval = 0.5f;
		attackHitBox = { 0.f, 0.f, 0.f, 0.f };
		attackHitBoxCheck.setSize(attackHitBox.getSize());
		break;
	}
}

void Player::SetRemainingBullet(int remainingBullet)
{
	this->remainingBullet = remainingBullet;
}

int Player::GetRemainingBullet()
{
	return remainingBullet;
}

void Player::ThrowWeapon(sf::Vector2f lookDir)
{
	if (weaponStatus.weaponType == Weapon::WeaponType::None)
		return;
	sceneDevL->OnWeaponThrow(weaponStatus.weaponType, remainingBullet, lookDir, position);
	weaponStatus.weaponType = Weapon::WeaponType::None;
	SetWeaponStatus();
}

void Player::DropWeapon(sf::Vector2f hitDir)
{
	if (weaponStatus.weaponType == Weapon::WeaponType::None)
		return;
	sceneDevL->OnWeaponDrop(weaponStatus.weaponType, remainingBullet, hitDir, position);
	weaponStatus.weaponType = Weapon::WeaponType::None;
	SetWeaponStatus();
}

void Player::DropWeapon()
{



}

void Player::Attack()
{
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		AttackDefault();
		break;
	case Weapon::WeaponType::Bat:
		AttackBat();
		break;
	case Weapon::WeaponType::Knife:
		AttackKnife();
		break;
	case Weapon::WeaponType::Machinegun:
		AttackMachinegun();
		break;
	case Weapon::WeaponType::Shotgun:
		AttackShotgun();
		break;
	}
}

void Player::AttackDefault()
{

}

void Player::AttackBat()
{

}

void Player::AttackKnife()
{

}

void Player::AttackMachinegun()
{

}

void Player::AttackShotgun()
{

}
