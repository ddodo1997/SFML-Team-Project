#pragma once
#include "WeaponRanged.h"

class Machinegun : public WeaponRanged
{
protected:


public:
	Machinegun(std::string name);
	~Machinegun() = default;
	Machinegun(const Machinegun& other) = delete;
	Machinegun& operator=(const Machinegun& other) = delete;

	void Attack() override;

};

