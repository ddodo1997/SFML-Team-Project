#pragma once
#include "WeaponRanged.h"

class Shotgun : public WeaponRanged
{
protected:


public:
	Shotgun(std::string name);
	~Shotgun() = default;
	Shotgun(const Shotgun& other) = delete;
	Shotgun& operator=(const Shotgun& other) = delete;

	void Attack() override;

};

