#include "stdafx.h"
#include "Knife.h"

Knife::Knife(std::string name)
	: WeaponMelee(name)
{
	weaponType = WeaponType::Knife;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Knife::Attack()
{

}
