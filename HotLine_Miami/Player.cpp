#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "Wall2.h"
#include "Decoration.h"

Player::Player(const std::string& name)
	: GameObject(name)
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	prevPos = position;
	position = pos;
	body.setPosition(position);
	leg.setPosition(position);
	noiseCircle.setPosition(position);
	collisionBox.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	leg.setScale(scale);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		origin = Utils::SetOrigin(leg, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	leg.setOrigin(origin);
}

void Player::SetScene(SceneGame* sceneGame)
{
	this->sceneGame = sceneGame;
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;

	//aniClipMap.insert("", )
}

void Player::Release()
{
	executingEnemy = nullptr;
}

void Player::Reset()
{
	isAlive = true;
	isOnPound = false;
	isExecuting = false;
	executionTimer = 10.f;
	executionCount = 10;
	speed = 100.f;
	onDieSpeed = 100.f;
	onDieEffectAccumTime = 0.6f;
	position = { 50,150 };
	SetScale({ 1.f, 1.f });
	animatorBody.SetTarget(&body);
	animatorLeg.SetTarget(&leg);
	SetOrigin(Origins::MC);
	sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
	walls = sceneGame->GetWalls();
	decorations = sceneGame->GetDecorations();

	enemies = sceneGame->GetEnemies();
	collisionBox.setSize({ 10.f,10.f });
	Utils::SetOrigin(collisionBox, Origins::MC);
	SetWeaponStatus();
	noiseCircle.setRadius(weaponStatus.noiseRadius);

	attackHitBoxCheck.setFillColor(sf::Color::Transparent);
	attackHitBoxCheck.setOutlineColor(sf::Color::Green);
	attackHitBoxCheck.setOutlineThickness(1.f);
	Utils::SetOrigin(attackHitBoxCheck, Origins::ML);

	weaponStatus.weaponType = Weapon::WeaponType::None;
	weaponStatus = WEAPON_TABLE->Get(weaponStatus.weaponType);
	attackHitBoxCheck.setSize({ weaponStatus.hitBoxWidth, weaponStatus.hitBoxHeight });
}

void Player::Update(float dt)
{
	if (attackTimer < weaponStatus.attackInterval)
	{
		attackTimer += dt;
	}
	else
	{
		if (attackTimer > weaponStatus.attackInterval)
			isMoving = false;
		attackTimer = Utils::Clamp(attackTimer, 0, weaponStatus.attackInterval);
		isAttacking = false;
	}

	hitBox.UpdateTr(body, body.getLocalBounds());
	attackHitBoxCheck.setPosition(body.getPosition());
	attackHitBoxCheck.setRotation(body.getRotation());
	Utils::SetOrigin(attackHitBoxCheck, Origins::ML);

	if (!isAlive)
	{
		isMoving = false;
		UpdateOnDie(dt);
		return;
	}

	animatorBody.Update(dt);
	animatorLeg.Update(dt);

	if (isOnPound)
	{
		
		if (executionCount > 0)
		{
			UpdateExecution(dt);
		}
		else
		{
			executingEnemy->SetStatus(Enemy::Status::Die);
			executingEnemy = nullptr;
			isOnPound = false;
		}
		return;
	}		

	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);

	if (mag > 1.f)
	{
		Utils::Normalize(direction);
	}
	leg.setRotation(Utils::Angle(direction));

	auto mPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());
	look = mPos - position;
	body.setRotation(Utils::Angle(look));

	Utils::SetOrigin(leg, Origins::MC);
	Utils::SetOrigin(body, Origins::MC);

	if (mag > 0)
	{
		if (!isMoving)
		{
			animatorLeg.Play("animations/Player/pLegAni.json");
			animatorLeg.SetSpeed(-1.f);
			if (!isAttacking)
			{
				UpdateBodyAnimationMoving();
				//animatorBody.Play("animations/Player/pBodyAni_default.json");
			}
			isMoving = true;
		}
	}
	else
	{
		if (!isAttacking)
		{
			UpdateBodyAnimationMoving();
			//animatorBody.Play("animations/Player/pBodyAni_default.json");
		}
		isMoving = false;
	}


	if (InputMgr::GetMouseButton(sf::Mouse::Left))
	{
		if (attackTimer >= weaponStatus.attackInterval)
		{
			Attack();
			attackTimer = 0;
		}
	}
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
	{
		if (sceneGame == nullptr)
			SetScene((SceneGame*)SCENE_MGR.GetCurrentScene());
		ThrowWeapon(look);
		weaponStatus.weaponType = Weapon::WeaponType::None;
		TryPickUpWeapon();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		if (sceneGame == nullptr)
			SetScene((SceneGame*)SCENE_MGR.GetCurrentScene());
		TryExecute();
	}



	SetPosition(position + direction * speed * dt);
	hitBox.UpdateTr(body, body.getLocalBounds());

}

void Player::UpdateBodyAnimationMoving()
{
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		animatorBody.Play("animations/Player/pBodyAni_default.json");
		if (isFlipped)
		{
			body.setScale(1.f, -1.f);
		}
		else
		{
			body.setScale(1.f, 1.f);
		}
		break;
	case Weapon::WeaponType::Bat:
		animatorBody.Play("animations/Player/pBodyAni_Bat.json");
		if (isFlipped)
		{
			body.setScale(1.f, -1.f);
		}
		else
		{
			body.setScale(1.f, 1.f);
		}
		break;
	case Weapon::WeaponType::Knife:
		animatorBody.Play("animations/Player/pBodyAni_Knife.json");
		if (isFlipped)
		{
			body.setScale(1.f, -1.f);
		}
		else
		{
			body.setScale(1.f, 1.f);
		}
		break;
	case Weapon::WeaponType::Machinegun:
		animatorBody.Play("animations/Player/pBodyAni_Machinegun.json");
		body.setScale(1.f, 1.f);
		break;
	case Weapon::WeaponType::Shotgun:
		animatorBody.Play("animations/Player/pBodyAni_Shotgun.json");
		body.setScale(1.f, 1.f);
		break;
	}
}

void Player::UpdateExecution(float dt)
{
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		UpdateExecutionDefualt(dt);
		break;
	case Weapon::WeaponType::Bat:
		UpdateExecutionBat(dt);
		break;
	case Weapon::WeaponType::Knife:
		UpdateExecutionKnife(dt);
		break;
	} 	
}

void Player::UpdateExecutionDefualt(float dt)
{
	if (isExecuting)
	{
		if (executionTimer > 0)
		{
			executionTimer -= dt;
			executionTimer = Utils::Clamp(executionTimer, 0.f, 10.f);
			//animatorBody.Play("animations/Player/Execution/pExctDefault.json");
		}
		else
		{
			executionCount--;
			isExecuting = false;
			// + �� Ƣ�� ����Ʈ ���?
		}
	}
	else
	{
		executionTimer = 10.f;
		animatorBody.Play("animations/Player/Execution/pExctDefault.json");
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			isExecuting = true;
			executionTimer = 0.4f;
		}
	}
}

void Player::UpdateExecutionBat(float dt)
{
	if (isExecuting)
	{
		if (executionTimer > 0)
		{
			executionTimer -= dt;
			executionTimer = Utils::Clamp(executionTimer, 0.f, 10.f);
			//animatorBody.Play("animations/Player/Execution/pExctBat.json");
		}
		else
		{
			executionCount--;
			isExecuting = false;
			// + �� Ƣ�� ����Ʈ ���?
		}
	}
	else
	{
		executionTimer = 10.f;
		animatorBody.Play("animations/Player/Execution/pExctBat.json");
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			isExecuting = true;
			executionTimer = 0.4f;
		}
	}
}

void Player::UpdateExecutionKnife(float dt)
{
	if (isExecuting)
	{
		if (executionTimer > 0)
		{
			executionTimer -= dt;
			executionTimer = Utils::Clamp(executionTimer, 0.f, 10.f);
			//animatorBody.Play("animations/Player/Execution/pExctKnife.json");
		}
		else
		{
			executionCount--;
			isExecuting = false;
		}
	}
}

void Player::FixedUpdate(float dt)
{
	if (isAttacking)
	{
		std::vector<Enemy*> enemies = sceneGame->GetEnemies();
		for (auto enemy : enemies)
		{
			if (enemy != nullptr)
			{
				if (enemy->GetStatus() != Enemy::Status::Die && enemy->GetStatus() != Enemy::Status::Stun)
				{
					if (attackHitBoxCheck.getGlobalBounds().intersects(enemy->GetGlobalBounds()))
					{
						enemy->OnHit(weaponStatus, look);
					}
				}
			}
		}
	}

	for (auto wall : walls)
	{
		auto wallBounds = wall->GetGlobalBounds();
		if (wallBounds.intersects(collisionBox.getGlobalBounds()))
		{
			position = prevPos;
		}
	}

	for (auto deco : decorations)
	{
		auto decoBounds = deco->GetGlobalBounds();
		if (decoBounds.intersects(collisionBox.getGlobalBounds()))
		{

			position = prevPos;
		}
	}
	SetPosition(position);
}

void Player::UpdateOnDie(float dt)
{
	if (onDieEffectAccumTime > 0)
		onDieEffectAccumTime -= dt;
	onDieEffectAccumTime = Utils::Clamp(onDieEffectAccumTime, 0.f, 3.f);

	SetPosition(position + direction * onDieSpeed * onDieEffectAccumTime * dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	if (!isAlive)
	{

	}
	else if (isMoving && !isOnPound)
		window.draw(leg);
	window.draw(body);
	hitBox.Draw(window);
	if (Variables::isDrawHitBox)
	{
		window.draw(attackHitBoxCheck);
		window.draw(collisionBox);
	}
}

void Player::OnHit(Weapon::WeaponStatus weapon, sf::Vector2f hitDir)
{
	auto normDir = Utils::GetNormal(hitDir);
	switch (weapon.weaponType)
	{
	case Weapon::WeaponType::Bat:
		OnHitByBat(normDir);
		break;
	case Weapon::WeaponType::Knife:
		OnHitByKnife(normDir);
		break;
	case Weapon::WeaponType::Machinegun:
		OnHitByMachinegun(normDir);
		break;
	case Weapon::WeaponType::Shotgun:
		OnHitByShotgun(normDir);
		break;
	}
	isAlive = false;
	if (weaponStatus.weaponType != Weapon::WeaponType::None)
		DropWeapon(normDir);
}

void Player::OnHitByBat(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(0, 4);
	tempTexId = "graphics/player/Die/sprPBackBlunt2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::OnHitByKnife(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(0, 2);
	tempTexId = "graphics/player/Die/sprPBackCut2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::OnHitByMachinegun(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(1, 4);
	tempTexId = "graphics/player/Die/sprPBackMachinegun2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::OnHitByShotgun(sf::Vector2f hitDir)
{
	std::string tempTexId;
	int tempIndex = Utils::RandomRange(1, 5);
	tempTexId = "graphics/player/Die/sprPBackShotgun2_" + std::to_string(tempIndex) + ".png";
	animatorBody.Stop();
	body.setTexture(TEXTURE_MGR.Get(tempTexId));
	body.setTextureRect({ 0,0,60,60 });
	SetRotation(Utils::Angle(hitDir));
	direction = hitDir;
	Utils::SetOrigin(body, Origins::MC);
}

void Player::TryPickUpWeapon()
{
	auto weapons = sceneGame->GetActiveWeapons();
	for (auto weapon : weapons)
	{
		if (GetHitBox().rect.getGlobalBounds().intersects(weapon->GetHitBox().rect.getGlobalBounds()))
		{
			WeaponPickUp(weapon->GetStatus());
			weapon->SetActive(false);
			return;
		}
	}
}

void Player::WeaponPickUp(Weapon::WeaponStatus weapon)
{
	weaponStatus = weapon;
	noiseCircle.setRadius(weapon.noiseRadius);
	Utils::SetOrigin(noiseCircle, Origins::MC);
	attackHitBoxCheck.setSize({ weaponStatus.hitBoxWidth, weaponStatus.hitBoxHeight });
	attackTimer = weaponStatus.attackInterval;
	UpdateBodyAnimationMoving();
}

void Player::SetWeaponStatus()
{
	Weapon::WeaponType tempWeaponType;
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		tempWeaponType = Weapon::WeaponType::None;
		break;
	case Weapon::WeaponType::Bat:
		tempWeaponType = Weapon::WeaponType::Bat;
		break;
	case Weapon::WeaponType::Knife:
		tempWeaponType = Weapon::WeaponType::Knife;
		break;
	case Weapon::WeaponType::Machinegun:
		tempWeaponType = Weapon::WeaponType::Machinegun;
		break;
	case Weapon::WeaponType::Shotgun:
		tempWeaponType = Weapon::WeaponType::Shotgun;
		break;
	}
	weaponStatus = WEAPON_TABLE->Get(tempWeaponType);
	attackHitBoxCheck.setSize({ weaponStatus.hitBoxWidth, weaponStatus.hitBoxHeight });
}

int Player::GetRemainingBullet()
{
	return weaponStatus.remainingBullet;
}

void Player::ThrowWeapon(sf::Vector2f lookDir)
{
	if (weaponStatus.weaponType == Weapon::WeaponType::None)
		return;
	sceneGame->OnWeaponThrow(weaponStatus, lookDir, position);
	weaponStatus.weaponType = Weapon::WeaponType::None;

	weaponStatus = WEAPON_TABLE->Get(weaponStatus.weaponType);

	Utils::SetOrigin(noiseCircle, Origins::MC);
	SetWeaponStatus();
	noiseCircle.setRadius(weaponStatus.noiseRadius);
}

void Player::DropWeapon(sf::Vector2f hitDir)
{
	if (weaponStatus.weaponType == Weapon::WeaponType::None)
		return;
	sceneGame->OnWeaponDrop(weaponStatus, position);
	weaponStatus.weaponType = Weapon::WeaponType::None;

	weaponStatus = WEAPON_TABLE->Get(weaponStatus.weaponType);

	SetWeaponStatus();
	noiseCircle.setRadius(weaponStatus.noiseRadius);
}

void Player::TryExecute()
{
	std::vector<Enemy*> enemies = sceneGame->GetEnemies();

	for (auto enemy : enemies)
	{
		if (enemy->GetStatus() == Enemy::Status::Stun)
		{
			if (GetHitBox().rect.getGlobalBounds().intersects(enemy->GetHitBox().rect.getGlobalBounds()))
			{
				enemy->SetStatus(Enemy::Status::Pounded); // ��⿡ ó������ �������� ���� // Status �߰���
				look = enemy->GetDirection();
				SetPosition(enemy->GetPosition());
				executingEnemy = enemy;
				Execute();
			}
		}
	}
}

void Player::Execute()
{
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		ExecuteDefault();
		break;
	case Weapon::WeaponType::Bat:
		ExecuteBat();
		break;
	case Weapon::WeaponType::Knife:
		ExecuteKnife();
		break;
	case Weapon::WeaponType::Machinegun:
		ExecuteMachinegun();
		break;
	case Weapon::WeaponType::Shotgun:
		ExecuteShotgun();
		break;
	}
	isOnPound = true;
	executionTimer = 0.4f;
}

void Player::ExecuteDefault()
{
	executionCount = Utils::RandomRange(3,4);
	isExecuting = false;
}

void Player::ExecuteBat()
{
	executionCount = 2;
	isExecuting = false;
}

void Player::ExecuteKnife()
{
	executionCount = 1;
	isExecuting = true;
	executionTimer = 0.4f;
	animatorBody.Play("animations/Player/Execution/pExctKnife.json");
}

void Player::ExecuteMachinegun()
{
	DropWeapon();
	ExecuteDefault();
}

void Player::ExecuteShotgun()
{
	DropWeapon();
	ExecuteDefault();
}

void Player::Attack()
{
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		AttackDefault();
		break;
	case Weapon::WeaponType::Bat:
		AttackBat();
		break;
	case Weapon::WeaponType::Knife:
		AttackKnife();
		break;
	case Weapon::WeaponType::Machinegun:
		AttackMachinegun();
		break;
	case Weapon::WeaponType::Shotgun:
		AttackShotgun();
		break;
	}
}

void Player::AttackDefault()
{
	animatorBody.Play("animations/Player/Attack/pAttackPunch.json");
	isAttacking = true;
	isFlipped = !isFlipped;
	if (isFlipped)
	{
		body.setScale(1.f, -1.f);
	}
	else
	{
		body.setScale(1.f, 1.f);
	}
}

void Player::AttackBat()
{
	animatorBody.Play("animations/Player/Attack/pAttackBat.json");
	isAttacking = true;
	isFlipped = !isFlipped;
	if (isFlipped)
	{
		body.setScale(1.f, -1.f);
	}
	else
	{
		body.setScale(1.f, 1.f);
	}
}

void Player::AttackKnife()
{
	animatorBody.Play("animations/Player/Attack/pAttackKnife.json");
	isAttacking = true;
	isFlipped = !isFlipped;
	if (isFlipped)
	{
		body.setScale(1.f, -1.f);
	}
	else
	{
		body.setScale(1.f, 1.f);
	}
}

void Player::AttackMachinegun()
{
	if (weaponStatus.remainingBullet > 0)
	{
		sceneGame->SpawnBullet()->Fire(Utils::AngleSpread(look, 10), this, weaponStatus);
		for (auto enemy : enemies)
		{
			if (enemy->GetGlobalBounds().intersects(noiseCircle.getGlobalBounds()) && !enemy->isDie() && !enemy->isStun())
			{
				enemy->SetStatus(Enemy::Status::Aggro);
			}
		}
		weaponStatus.remainingBullet--;
		animatorBody.Play("animations/Player/Attack/pAttackMachinegun.json");
		isAttacking = true;
	}
	else
	{
		// ���л���?
	}
}

void Player::AttackShotgun()
{
	if (weaponStatus.remainingBullet > 0)
	{
		for (int i = 0; i < 6; i++)
			sceneGame->SpawnBullet()->Fire(Utils::AngleSpread(look, 10), this, weaponStatus);

		for (auto enemy : enemies)
		{
			if (enemy->GetGlobalBounds().intersects(noiseCircle.getGlobalBounds()) && !enemy->isDie() && !enemy->isStun())
			{
				enemy->SetStatus(Enemy::Status::Aggro);
			}
		}
		weaponStatus.remainingBullet--;
		animatorBody.Play("animations/Player/Attack/pAttackShotgun.json");
		isAttacking = true;
	}
	else
	{
		// ���л���?
	}
}
