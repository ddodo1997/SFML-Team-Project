#include "stdafx.h"
#include "Machinegun.h"

Machinegun::Machinegun(std::string name)
	:WeaponRanged(name)
{
	weaponType = WeaponType::Machinegun;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Machinegun::Attack()
{
}
