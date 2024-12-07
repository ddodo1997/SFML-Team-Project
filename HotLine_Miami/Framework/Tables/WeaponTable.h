#pragma once
#include "Weapon.h"

class Weapon;

class WeaponTable : public DataTable
{
public:
	static Weapon::WeaponStatus Undefined;

protected:
	std::unordered_map<Weapon::WeaponType, Weapon::WeaponStatus> table;
	std::string filePath = "tables/weaponTable.json";

public:
	WeaponTable() : DataTable(DataTable::Types::Weapon) { };
	~WeaponTable() = default;

	bool Load() override;
	void Release() override;
	void SetPath();

	const Weapon::WeaponStatus& Get(Weapon::WeaponType& id);
	Weapon::WeaponType IndexTo(const std::string& type);
};