#include "stdafx.h"
#include "Weapon.h"
#include "Enemy.h"
#include "SceneGame.h"
#include <chrono>
#include "time.h"
int Weapon::indexCounter = 0;

Weapon::Weapon(std::string name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	Utils::SetOrigin(weaponSprite, Origins::MC);

	indexNo = indexCounter++;

	// 인덱스 부여 관련함수 미완성, 현재 std::stoi에서 범위초과로 런타임 에러 발생
	//auto timer = std::chrono::system_clock::now();
	//auto t = timer.time_since_epoch();
	//int tempIndex = std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	//std::string tempStr = std::to_string(tempIndex) + std::to_string(indexCounter++);
	//indexNo = std::stoi(tempStr);
}

void Weapon::Init()
{

}

void Weapon::Release()
{

}

void Weapon::Reset()
{
	scenePointer = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	isPickupable = false;
}

void Weapon::Update(float dt)
{
	hitBox.UpdateTr(weaponSprite, weaponSprite.getLocalBounds());

	if(onDropTimer > 0.f)
		onDropTimer -= dt;
	onDropTimer = Utils::Clamp(onDropTimer, 0.f, 3.f);
	if(onThrowTimer > 0.f)
		onThrowTimer -= dt;
	onThrowTimer = Utils::Clamp(onThrowTimer, 0.f, 3.f);

	if (onThrowTimer <= 0.f && onDropTimer <= 0.f)
		isPickupable = true;

	float rotationMultiplierOnDrop = 5.f;
	float rotationMultiplierOnThrow = 6.f;

	SetRotation(rotation + speedOnDrop * onDropTimer * dt * rotationMultiplierOnDrop + speedOnThrow * onThrowTimer * dt * rotationMultiplierOnThrow);
	SetPosition(position + speedOnDrop * onDropTimer * direction * dt + speedOnThrow * onThrowTimer * direction * dt);
	Utils::SetOrigin(weaponSprite, Origins::MC);
}

void Weapon::FixedUpdate(float dt)
{
	// 플레이어에게 던져진 무기의 경우 아래 코드로 적들과 충돌처리	
	
	if (!(onThrowTimer > 0.f))
	{
		return;
	}
	auto eList = scenePointer->GetEnemies();
	for (auto enemy : eList)
	{
		if (weaponSprite.getGlobalBounds().intersects(enemy->GetGlobalBounds()) && !enemy->isDie() && !enemy->isStun())
		{
			enemy->OnHit(weaponStatus, direction, true);
		}
	}
}

void Weapon::Draw(sf::RenderWindow& window)
{
	window.draw(weaponSprite);
	hitBox.Draw(window);
}

void Weapon::SetPosition(sf::Vector2f pos)
{
	position = pos;
	weaponSprite.setPosition(position);
}

void Weapon::SetRotation(float angle)
{
	rotation = angle;
	weaponSprite.setRotation(rotation);
}

void Weapon::SetOrigin(Origins origin)
{
	originPreset = origin;
	if(origin != Origins::Custom)
		this->origin = Utils::SetOrigin(weaponSprite, originPreset);
}

void Weapon::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	this->origin = newOrigin;
	weaponSprite.setOrigin(origin);
}

void Weapon::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	weaponSprite.setScale(scale);
}

/// <summary>
/// WeaponStatus Set 함과 동시에 setTexture.
/// </summary>
/// <param name="weapon"></param>
void Weapon::SetStatus(const WeaponStatus& status)
{
	this->weaponStatus = status;
	weaponSprite.setTexture(TEXTURE_MGR.Get(weaponStatus.textureId));
	SetScale({ 1.f, 1.f });
	SetOrigin(Origins::MC);
}

void Weapon::OnPickUp()
{

}

void Weapon::OnThrow(sf::Vector2f direction)
{
	onDropTimer = 0.f;
	onThrowTimer = 2.f;
	onThrowTimer = Utils::RandomRange(1.9f, 2.1f);
	this->direction = Utils::AngleSpread(direction, 10);
}

void Weapon::OnDrop(sf::Vector2f direction)
{
	onThrowTimer = 0.f;
	onDropTimer = 1.f;
	onDropTimer = Utils::RandomRange(0.2f, 1.0f);
	this->direction = Utils::DegreeToNormal(Utils::RandomRange(0,360));
}

void Weapon::OnHitWall()
{
	onThrowTimer *= 0.3f;
	onDropTimer *= 0.3f;
}

void Weapon::SetWeaponType(WeaponType type)
{
	weaponStatus = WEAPON_TABLE->Get(type);
	weaponSprite.setTexture(TEXTURE_MGR.Get(weaponStatus.textureId));
}

Weapon::WeaponType Weapon::GetWeaponType()
{
	return weaponStatus.weaponType;
}

bool Weapon::GetIsRanged()
{
	return weaponStatus.isRangedWeapon;
}

bool Weapon::GetIsPickupable()
{
	return isPickupable;
}
