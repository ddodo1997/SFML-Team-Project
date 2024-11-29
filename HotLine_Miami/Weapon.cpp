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

	float rotationMultiplierForTest = 1000.f;

	SetRotation(rotation + speedOnDrop * onDropTimer * dt * rotationMultiplierForTest);
	SetPosition(position + speedOnDrop * onDropTimer * direction * dt + speedOnThrow * onThrowTimer * direction * dt);
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
	//onThrowTimer = Utils::RandomRange(0.5f, 1.5f);
	onThrowTimer = 1.f;
	this->direction = Utils::GetNormal(direction);
}

void Weapon::OnDrop(sf::Vector2f direction)
{
	onThrowTimer = 0.f;
	//onDropTimer = Utils::RandomRange(0.5f, 1.5f);
	onDropTimer = 1.f;
	this->direction = Utils::GetNormal(direction);
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
	return 0;
}
