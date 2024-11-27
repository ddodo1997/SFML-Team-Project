#pragma once
#include "Weapon.h"
class WeaponRanged : public Weapon
{
protected:


public:
    WeaponRanged(std::string name = "");
    ~WeaponRanged() = default;
    WeaponRanged(const WeaponRanged& other) = delete;
    WeaponRanged& operator=(const WeaponRanged& other) = delete;

    virtual void Attack() override = 0;
};

