#include "stdafx.h"
#include "UiHudL.h"

UiHudL::UiHudL(const std::string& name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void UiHudL::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiHudL::SetRotation(float angle)
{
	rotation = angle;
}

void UiHudL::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiHudL::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiHudL::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiHudL::Init()
{
	Reset();

}

void UiHudL::Release()
{
}

void UiHudL::Reset()
{	
	currentWeaponStatus.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	currentWeaponStatus.setCharacterSize(30.f);
	currentWeaponStatus.setFillColor(sf::Color::White);
	currentWeaponStatus.setString(std::to_string(InputMgr::GetMousePosition().y));
	currentWeaponStatus.setPosition({ 30.f,30.f });
	Utils::SetOrigin(currentWeaponStatus, Origins::TL);

	volumeDisplayer.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	volumeDisplayer.setCharacterSize(30.f);
	volumeDisplayer.setFillColor(sf::Color::White);
	volumeDisplayer.setString(std::to_string(InputMgr::GetMousePosition().y));
	volumeDisplayer.setPosition({ 30.f,280.f });
	Utils::SetOrigin(volumeDisplayer, Origins::TL);

	cursorAnimator.SetTarget(&mouseSprite);
	cursorAnimator.Play("animations/cursor.json");
	mouseSprite.setScale(3.f, 3.f);
	Utils::SetOrigin(mouseSprite, Origins::MC);
}

void UiHudL::Update(float dt)
{
	UpdateVolumeDisplay(dt);
	UpdateMouseSprite(dt);
}

void UiHudL::UpdateMouseSprite(float dt)
{
	if(!(cursorAnimator.IsPlaying()))
		cursorAnimator.Play("animations/cursor.json");
	mouseSprite.setPosition(VIEW_MGR.GetMouseSpritePos());
	Utils::SetOrigin(mouseSprite, Origins::MC);
	cursorAnimator.Update(dt);
}

void UiHudL::UpdateWeaponStatus(Weapon::WeaponStatus weaponStatus, int remainingBullet)
{
	weaponTypeIndex = (int)weaponStatus.weaponType;
	std::string weaponId;
	if ((int)weaponStatus.weaponType == -1)
	{
		weaponId = "None";
	}
	if ((int)weaponStatus.weaponType == 0)
	{
		weaponId = "Bat";
	}
	if ((int)weaponStatus.weaponType == 1)
	{
		weaponId = "Knife";
	}
	if ((int)weaponStatus.weaponType == 2)
	{
		weaponId = "Machinegun";
	}
	if ((int)weaponStatus.weaponType == 3)
	{
		weaponId = "Shotgun";
	}
	std::string bulletStatus = "\nNo guns";
	if (weaponStatus.isRangedWeapon)
		bulletStatus = "\nBullets : " + std::to_string(remainingBullet)
		+ " \/ " + std::to_string(weaponStatus.maxBullet);
	currentWeaponStatus.setString("Weapon ID: " + weaponId + bulletStatus);
	Utils::SetOrigin(currentWeaponStatus, Origins::TL);
}

void UiHudL::UpdateVolumeDisplay(float dt)
{
	std::string bgmVol = std::to_string((int)SOUND_MGR.GetBgmVolume());
	std::string sfxVol = std::to_string((int)SOUND_MGR.GetSfxVolume());
	
	volumeDisplayer.setString("Bgm Vol : " + bgmVol + "\nSfx Vol : " + sfxVol);
}

void UiHudL::Draw(sf::RenderWindow& window)
{
	window.draw(currentWeaponStatus);
	window.draw(volumeDisplayer);
	if (weaponTypeIndex != -1)
	{
		window.draw(currentWeaponStatus);
	}
	window.draw(mouseSprite);
}
