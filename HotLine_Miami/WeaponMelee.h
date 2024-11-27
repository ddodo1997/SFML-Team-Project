#pragma once
#include "Weapon.h"
class WeaponMelee : public Weapon
{
protected:



public:
    WeaponMelee(std::string name = "");
    ~WeaponMelee() = default;
    WeaponMelee(const WeaponMelee& other) = delete;
    WeaponMelee& operator=(const WeaponMelee& other) = delete;

    virtual void Attack() override = 0;
};

