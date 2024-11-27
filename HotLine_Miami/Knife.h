#pragma once
#include "WeaponMelee.h"
class Knife : public WeaponMelee
{
protected:



public:
	Knife(std::string name);
	~Knife() = default;
	Knife(const Knife& other) = delete;
	Knife& operator=(const Knife& other) = delete;

	void Attack() override;

};

