#pragma once
#include "WeaponMelee.h"
class Bat : public WeaponMelee
{
protected:


public:
	Bat(std::string name);
	~Bat() = default;
	Bat(const Bat& other) = delete;
	Bat& operator=(const Bat& other) = delete;

	void Attack() override;

};

