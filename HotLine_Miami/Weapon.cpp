#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(std::string name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	Utils::SetOrigin(weaponSprite, Origins::MC);
}

void Weapon::Init()
{

}

void Weapon::Release()
{

}

void Weapon::Reset()
{
	isPickupable = false;
}

void Weapon::Update(float dt)
{
	hitBox.UpdateTr(weaponSprite, weaponSprite.getLocalBounds());

	if(onDropTimer > 0.f)
		onDropTimer -= dt;
	onDropTimer = Utils::Clamp(onDropTimer, 0.f, 3.f);
	if(onThrowTimer > 0.f)
		onThrowTimer -= dt;
	onThrowTimer = Utils::Clamp(onThrowTimer, 0.f, 3.f);

	if (onThrowTimer <= 0.f && onDropTimer <= 0.f)
		isPickupable = true;

	float rotationMultiplierOnDrop = 5.f;
	float rotationMultiplierOnThrow = 5.f;

	SetRotation(rotation + speedOnDrop * onDropTimer * dt * rotationMultiplierOnDrop + speedOnThrow * onThrowTimer * dt * rotationMultiplierOnThrow);
	SetPosition(position + speedOnDrop * onDropTimer * direction * dt + speedOnThrow * onThrowTimer * direction * dt);
	Utils::SetOrigin(weaponSprite, Origins::MC);
}

void Weapon::Draw(sf::RenderWindow& window)
{
	window.draw(weaponSprite);
	hitBox.Draw(window);
}

void Weapon::SetPosition(sf::Vector2f pos)
{
	position = pos;
	weaponSprite.setPosition(position);
}

void Weapon::SetRotation(float angle)
{
	rotation = angle;
	weaponSprite.setRotation(rotation);
}

void Weapon::OnPickUp()
{

}

void Weapon::OnThrow(sf::Vector2f direction)
{
	onDropTimer = 0.f;
	onThrowTimer = 2.f;
	onThrowTimer = Utils::RandomRange(1.9f, 2.1f);
	this->direction = Utils::AngleSpread(direction, 10);
}

void Weapon::OnDrop(sf::Vector2f direction)
{
	onThrowTimer = 0.f;
	onDropTimer = 1.f;
	onDropTimer = Utils::RandomRange(0.2f, 1.0f);
	this->direction = Utils::DegreeToNormal(Utils::RandomRange(0,360));
}

void Weapon::SetWeaponType(WeaponType type)
{
	weaponStatus = WEAPON_TABLE->Get(type);
	weaponSprite.setTexture(TEXTURE_MGR.Get(weaponStatus.textureId));
}

Weapon::WeaponType Weapon::GetWeaponType()
{
	return weaponStatus.weaponType;
}

bool Weapon::GetIsRanged()
{
	return weaponStatus.isRangedWeapon;
}

bool Weapon::GetIsPickupable()
{
	return isPickupable;
}

void Weapon::SetRemainingBullet(int bullet)
{
	remainingBullet = bullet;
}

int Weapon::GetRemainingBullet()
{
	return remainingBullet;
}
