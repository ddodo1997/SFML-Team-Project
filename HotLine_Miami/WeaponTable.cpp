#include "stdafx.h"
#include "WeaponTable.h"

Weapon::WeaponStatus WeaponTable::Undefined;

bool WeaponTable::Load()
{
	SetPath();
	Release();
	std::ifstream fStream(filePath);

	if (!fStream.is_open())
	{
		std::cerr << "Weapon Table File Open Error!!" << std::endl;
	}
	json data = json::parse(fStream);

	for (const auto& weapon : data["Weapons"])
	{
		Weapon::WeaponStatus newWeapon;
		newWeapon.weaponType = weapon["WeaponType"];
		newWeapon.textureId = weapon["TextureId"];
		newWeapon.damage = weapon["Damage"];
		newWeapon.damageOnThrow = weapon["DamageOnThrow"];
		newWeapon.isRangedWeapon = weapon["isRangedWeapon"];
		newWeapon.maxBullet = weapon["MaxBullet"];
		table.insert({ (Weapon::WeaponType)newWeapon.weaponType, newWeapon});
	}

	fStream.close();
	return true;
}

void WeaponTable::Release()
{
}

void WeaponTable::SetPath()
{
	switch (Variables::currentLang)
	{
	case Languages::English:
		filePath = "tables/weaponTable.json";
		break;
	}
}

const Weapon::WeaponStatus& WeaponTable::Get(Weapon::WeaponType& id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}
	return find->second;
}