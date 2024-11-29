#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(std::string name)
	: GameObject(name)
{

}

void Weapon::Init()
{
}

void Weapon::Release()
{
}

void Weapon::Reset()
{
}

void Weapon::Update(float dt)
{
}

void Weapon::Draw(sf::RenderWindow& window)
{
}

void Weapon::OnPickUp()
{

}

void Weapon::OnThrow(sf::Vector2f direction)
{

}

void Weapon::OnDrop(sf::Vector2f direction)
{

}

void Weapon::SetWeaponType(WeaponType type)
{
	weaponStatus = WEAPON_TABLE->Get(type);
}

Weapon::WeaponType Weapon::GetWeaponType()
{
	return weaponStatus.weaponType;
}

bool Weapon::GetIsRanged()
{
	return weaponStatus.isRangedWeapon;
}

void Weapon::SetRemainingBullet(int bullet)
{
	remainingBullet = bullet;
}

int Weapon::GetRemainingBullet()
{
	return 0;
}
