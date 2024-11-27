#include "stdafx.h"
#include "Shotgun.h"

Shotgun::Shotgun(std::string name)
	:WeaponRanged(name)
{
	weaponType = WeaponType::Machinegun;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Shotgun::Attack()
{

}
