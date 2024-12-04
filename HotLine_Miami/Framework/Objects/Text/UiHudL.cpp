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
	mPosX.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	mPosX.setCharacterSize(30.f);
	mPosX.setFillColor(sf::Color::White);
	mPosX.setString(std::to_string(InputMgr::GetMousePosition().x));
	mPosX.setPosition({ 30.f,30.f });
	Utils::SetOrigin(mPosX, Origins::TL);

	mPosY.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	mPosY.setCharacterSize(30.f);
	mPosY.setFillColor(sf::Color::White);
	mPosY.setString(std::to_string(InputMgr::GetMousePosition().y));
	mPosY.setPosition({ 30.f,80.f });
	Utils::SetOrigin(mPosY, Origins::TL);

	hitDirectionText.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	hitDirectionText.setCharacterSize(30.f);
	hitDirectionText.setFillColor(sf::Color::White);
	hitDirectionText.setString(std::to_string(InputMgr::GetMousePosition().y));
	hitDirectionText.setPosition({ 30.f,130.f });
	Utils::SetOrigin(hitDirectionText, Origins::TL);

	hitDirection.setTexture(TEXTURE_MGR.Get("graphics/ETC/sprArrowColor.png"));
	hitDirection.setTextureRect({ 0,0,16,16 });
	hitDirection.setScale(2.f, 2.f);
	hitDirection.setPosition({ 38.f,230.f });
	Utils::SetOrigin(hitDirection, Origins::ML);

	lookDirectionText.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	lookDirectionText.setCharacterSize(30.f);
	lookDirectionText.setFillColor(sf::Color::White);
	lookDirectionText.setString(std::to_string(InputMgr::GetMousePosition().y));
	lookDirectionText.setPosition({ 30.f,330.f });
	Utils::SetOrigin(lookDirectionText, Origins::TL);

	lookDirection.setTexture(TEXTURE_MGR.Get("graphics/ETC/sprArrowColor.png"));
	lookDirection.setTextureRect({ 0,0,16,16 });
	lookDirection.setScale(2.f, 2.f);
	lookDirection.setPosition({ 38.f,430.f });
	Utils::SetOrigin(lookDirection, Origins::ML);

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
	UpdateMPos(dt);
	UpdateVolumeDisplay(dt);
	UpdateMouseSprite(dt);
}

void UiHudL::UpdateMPos(float dt)
{
	mPosX.setString(std::to_string(InputMgr::GetMousePosition().x));
	Utils::SetOrigin(mPosX, Origins::TL);

	mPosY.setString(std::to_string(InputMgr::GetMousePosition().y));
	Utils::SetOrigin(mPosY, Origins::TL);
}

void UiHudL::UpdateMouseSprite(float dt)
{
	if(!(cursorAnimator.IsPlaying()))
		cursorAnimator.Play("animations/cursor.json");
	mouseSprite.setPosition(VIEW_MGR.GetMouseSpritePos());
	Utils::SetOrigin(mouseSprite, Origins::MC);
	cursorAnimator.Update(dt);
}

void UiHudL::UpdateHitDir(sf::Vector2f hitDir)
{
	hitDirectionText.setString(std::to_string(hitDir.x) + "\n" + std::to_string(hitDir.y));
	Utils::SetOrigin(hitDirectionText, Origins::TL);

	hitDirection.setRotation(Utils::Angle(hitDir)-90);
	Utils::SetOrigin(hitDirection, Origins::MC);
}

void UiHudL::UpdateLookDir(sf::Vector2f lookDir)
{
	lookDirectionText.setString(std::to_string(lookDir.x) + "\n" + std::to_string(lookDir.y));
	Utils::SetOrigin(hitDirectionText, Origins::TL);

	lookDirection.setRotation(Utils::Angle(lookDir) - 90);
	Utils::SetOrigin(hitDirection, Origins::MC);
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
	//window.draw(mPosX);
	//window.draw(mPosY);
	//window.draw(hitDirectionText);
	//window.draw(hitDirection);
	//window.draw(lookDirectionText);
	//window.draw(lookDirection);
	window.draw(currentWeaponStatus);
	window.draw(volumeDisplayer);
	if (weaponTypeIndex != -1)
	{
		window.draw(currentWeaponStatus);
	}
	window.draw(mouseSprite);
}
