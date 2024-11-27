#include "stdafx.h"
#include "Bat.h"

Bat::Bat(std::string name)
	: WeaponMelee(name)
{
	weaponType = WeaponType::Bat;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Bat::Attack()
{

}
