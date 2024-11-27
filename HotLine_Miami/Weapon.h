#pragma once
#include "GameObject.h"
class Weapon : public GameObject
{
public: 
	enum class WeaponType
	{
		None = -1,
		Bat,
		Knife,
		Machinegun,
		Shotgun,
	};
	
protected:
	WeaponType		weaponType;
	int				damage;
	int				damageOnThrow;

public:
	Weapon(std::string name);
	~Weapon() = default;
	Weapon(const Weapon& other) = delete;
	Weapon& operator=(const Weapon& other) = delete;

	virtual void Attack() = 0;
};

