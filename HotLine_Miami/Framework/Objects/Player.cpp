#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "Wall2.h"
#include "Decoration.h"
#include "Boss1.h"
#include "Boss2.h"

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
	// Mask Test용
	ResetMask();	
	// ~Mask Test용

	isMoving = false;
	isAlive = true;
	isAttacking = false;
	isOnPound = false;
	isExecuting = false;
	isControlable = true;
	isExecutionOnWall = false;
	isFlipped = false;
	isPoundingBoss = false;
	executionTimer = 10.f;
	executionCount = 10;
	speed = 130;
	onDieSpeed = 300;
	onDieEffectAccumTime = 0.6f;
	bulletProofCount = 0;
	position = { 50.f, 100.f };
	SetScale({ 1.f, 1.f });
	body.setTexture(TEXTURE_MGR.Get("graphics/player/Walk/pWalkUnArmdNoMask.png"));
	body.setTextureRect(sf::IntRect(0, 0, 32, 32));
	animatorBody.SetTarget(&body);
	animatorLeg.SetTarget(&leg);
	animatorLeg.Play("animations/Player/pLegAni.json");
	body.setTexture(TEXTURE_MGR.Get("graphics/player/Walk/pWalkUnArmdNoMask.png"));
	body.setTextureRect(sf::IntRect(0, 0, 32, 32));
	SetOrigin(Origins::MC);
	sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
	walls = sceneGame->GetWalls();
	decorations = sceneGame->GetDecorations();
	boss1 = sceneGame->GetBoss1();

	enemies = sceneGame->GetEnemies();
	collisionBox.setSize({ 10.f,10.f });
	Utils::SetOrigin(collisionBox, Origins::MC);
	SetWeaponStatus();
	noiseCircle.setRadius(weaponStatus.noiseRadius);
	noiseCircle.setFillColor(sf::Color::Transparent);
	noiseCircle.setOutlineColor(sf::Color::Blue);
	noiseCircle.setOutlineThickness(3);

	attackHitBoxCheck.setFillColor(sf::Color::Transparent);
	attackHitBoxCheck.setOutlineColor(sf::Color::Green);
	attackHitBoxCheck.setOutlineThickness(1.f);
	Utils::SetOrigin(attackHitBoxCheck, Origins::ML);

	weaponStatus.weaponType = Weapon::WeaponType::Bat;
	weaponStatus = WEAPON_TABLE->Get(weaponStatus.weaponType);
	attackHitBoxCheck.setSize({ weaponStatus.hitBoxWidth, weaponStatus.hitBoxHeight });

<<<<<<< HEAD
	isMoving = false;
	isAlive = true;
	isAttacking = false;
	isOnPound = false;
	isExecuting = false;
	isControlable = true;
	isExecutionOnWall = false;
	isFlipped = false;
	isPoundingBoss = false;
	executionTimer = 10.f;
	executionCount = 10;
	speed = 130;
	onDieSpeed = 300;
	onDieEffectAccumTime = 0.6f;
	bulletProofCount = 0;
=======
	Utils::SetOrigin(body, Origins::MC);
>>>>>>> origin/Dev_L
}

void Player::ResetMask(bool ifInitialSetting)
{
	switch (currentMask)
	{
	case Mask::None:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_x.png"));
		break;
	case Mask::Chicken:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_0.png"));
		break;
	case Mask::Tiger:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_2.png"));
		break;
	case Mask::Rabbit:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_5.png"));
		break;
	case Mask::Wolf:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_6.png"));
		break;
	case Mask::Giraffe:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_7.png"));
		break;
	case Mask::Elephant:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_9.png"));
		break;
	case Mask::Walrus:
		mask.setTexture(TEXTURE_MGR.Get("graphics/player/Masks/sprMasks_21.png"));
		break;
	}
	Utils::SetOrigin(mask, Origins::MC);

	if (ifInitialSetting)
	{
		switch (currentMask)
		{
		case Mask::None:

			break;
		case Mask::Chicken:
			
			break;
		case Mask::Tiger:
			
			break;
		case Mask::Rabbit:
			speed = 150.f;
			break;
		case Mask::Wolf:
			weaponStatus.weaponType = Weapon::WeaponType::Knife;
			SetWeaponStatus();
			break;
		case Mask::Giraffe:
			
			break;
		case Mask::Elephant:
			bulletProofCount = 1;
			break;
		case Mask::Walrus:
			bulletProofCount = 2;
			break;
		}
	}
}

void Player::Update(float dt)
{
	if (!IsControlable())
	{
		speed = 0.f;
		return;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Semicolon))
	{
		int currentMaskIndex = (int)currentMask;
		currentMaskIndex--;
		currentMaskIndex = Utils::Clamp(currentMaskIndex, 0, 7);
		currentMask = (Mask)currentMaskIndex;
		ResetMask();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Apostrophe))
	{
		int currentMaskIndex = (int)currentMask;
		currentMaskIndex++;
		currentMaskIndex = Utils::Clamp(currentMaskIndex, 0, 7);
		currentMask = (Mask)currentMaskIndex;
		ResetMask();
	}


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
	UpdateMask(dt);
	//if (isMoving)
	//{
	//	UpdateMask(dt);
	//}

	if (isOnPound)
	{
		if (isPoundingBoss)
		{			
			UpdateExecutionBoss1(dt);
		}
		else
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
	hitBox.UpdateTr(collisionBox, collisionBox.getLocalBounds());
}

void Player::UpdateBodyAnimationMoving()
{
	switch (weaponStatus.weaponType)
	{
	case Weapon::WeaponType::None:
		animatorBody.PlayP("animations/Player/pBodyAni_default.json");
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
		animatorBody.PlayP("animations/Player/pBodyAni_Bat.json");
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
		animatorBody.PlayP("animations/Player/pBodyAni_Knife.json");
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
		animatorBody.PlayP("animations/Player/pBodyAni_Machinegun.json");
		body.setScale(1.f, 1.f);
		break;
	case Weapon::WeaponType::Shotgun:
		animatorBody.PlayP("animations/Player/pBodyAni_Shotgun.json");
		body.setScale(1.f, 1.f);
		break;
	}
}

void Player::UpdateMask(float dt)
{
	sf::Transform bodyTrans = body.getTransform();

	maskRenderState = bodyTrans.translate((sf::Vector2f)animatorBody.GetCurrentMaskPos());

	//mask.setPosition(position);
	//mask.setRotation(Utils::Angle(look));
}

void Player::UpdateExecution(float dt)
{
	if (!isExecutionOnWall)
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
	else
	{
		UpdateExecutionWall(dt);
	}

}

void Player::UpdateExecutionBoss1(float dt)
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
			boss1->ChangePattern(Boss1::Patterns::Die);
			SOUND_MGR.PlaySfx("sound/Attack/sndHit.wav");
			animatorBody.PlayP("animations/Player/Execution/pExctBat_boss1.json");
			isOnPound = false;
			isPoundingBoss = false;
		}
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
		animatorBody.PlayP("animations/Player/Execution/pExctDefault.json");
		Utils::SetOrigin(body, Origins::MC);
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			isExecuting = true;
			executionTimer = 0.25f;
			SOUND_MGR.PlaySfx("sound/Attack/sndWeaponHit.wav");
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
		animatorBody.PlayP("animations/Player/Execution/pExctBat.json");
		Utils::SetOrigin(body, Origins::MC);
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			isExecuting = true;
			executionTimer = 0.25f;
			SOUND_MGR.PlaySfx("sound/Attack/sndWeaponHit.wav");
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
			SOUND_MGR.PlaySfx("sound/Attack/sndHit.wav");
		}
	}
}

void Player::UpdateExecutionWall(float dt)
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
			isExecutionOnWall = false;
			SOUND_MGR.PlaySfx("sound/Attack/sndHit.wav");
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
				if (!enemy->isDie() && !enemy->isStun() && !enemy->isStunOnWall())
				{
					if (attackHitBoxCheck.getGlobalBounds().intersects(enemy->GetGlobalBounds()))
					{
						if (!Utils::RayCast(position, Utils::GetNormal(enemy->GetPosition() - position), 100.f, enemy))
						{
							enemy->OnHit(weaponStatus, look);
							std::string sfxFilePath = "sound/Attack/sndWeaponHit.wav";
							if (weaponStatus.weaponType == Weapon::WeaponType::Knife)
								sfxFilePath = "sound/Attack/sndHit.wav";							
							if (currentMask == Mask::Tiger)
							{
								auto tempWeaponStatus = Weapon::WeaponType::Bat;
								enemy->OnHit(WEAPON_TABLE->Get(tempWeaponStatus), look);
							}
							SOUND_MGR.PlaySfx(sfxFilePath);
						}
					}
				}
			}
		}

		if (boss1 != nullptr)
		{
			if (weaponStatus.weaponType == Weapon::WeaponType::Bat)
			{	
				if (attackHitBoxCheck.getGlobalBounds().intersects(boss1->GetCollisionBox().getGlobalBounds()))
				{
					boss1->OnHit(look);
				}
			}
		}
	}
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
	window.draw(mask, maskRenderState);
	hitBox.Draw(window);
	if (Variables::isDrawHitBox)
	{
		window.draw(attackHitBoxCheck);
		window.draw(collisionBox);
		window.draw(noiseCircle);
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
	SOUND_MGR.PlaySfx("sound/Attack/sndPickupWeapon.wav");
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
	SOUND_MGR.PlaySfx("sound/Attack/sndThrow.wav");
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
				enemy->SetStatus(Enemy::Status::Pounded); 
				look = enemy->GetDirection();
				SetRotation(Utils::Angle(look));
				SetPosition(enemy->GetPosition());
				Utils::SetOrigin(body, Origins::MC);
				executingEnemy = enemy;
				Execute();
				return;
			}
		}
		if (enemy->GetStatus() == Enemy::Status::StunOnWall)
		{
			if (GetHitBox().rect.getGlobalBounds().intersects(enemy->GetHitBox().rect.getGlobalBounds()))
			{
				enemy->SetStatus(Enemy::Status::Die);
				look = enemy->GetDirection();
				SetRotation(Utils::Angle(look));
				SetPosition(enemy->GetPosition());
				Utils::SetOrigin(body, Origins::MC);
				executingEnemy = enemy;
				ExecuteAgainstWall();
				return;
			}
		}
	}

	if (boss1 != nullptr)
	{
		if (boss1->IsExcutable() && weaponStatus.weaponType == Weapon::WeaponType::Bat)
		{
			if (GetHitBox().rect.getGlobalBounds().intersects(boss1->GetCollisionBox().getGlobalBounds()))
			{
				boss1->ChangePattern(Boss1::Patterns::Pounded); // Pounded 추가시 로 변경 필요
				look = boss1->GetDirection();
				SetRotation(Utils::Angle(-look));
				SetPosition(boss1->GetPosition());
				Utils::SetOrigin(body, Origins::MC);
				executionCount = 1;
				isExecuting = true;
				isOnPound = true;
				isPoundingBoss = true;
				executionTimer = 0.5f;
				animatorBody.PlayP("animations/Player/Execution/pExctBat_boss1.json");
				return;
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

void Player::ExecuteAgainstWall()
{
	DropWeapon();
	isOnPound = true;
	executionCount = 1;
	isExecuting = true;
	isExecutionOnWall = true;
	executionTimer = 0.4f;
	animatorBody.PlayP("animations/Player/Execution/pExctWallKick.json");
	Utils::SetOrigin(body, Origins::MC);
}

void Player::ExecuteDefault()
{
	executionCount = Utils::RandomRange(3, 4);
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
	animatorBody.PlayP("animations/Player/Execution/pExctKnife.json");
	Utils::SetOrigin(body, Origins::MC);
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
	animatorBody.PlayP("animations/Player/Attack/pAttackPunch.json");
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
	int filePathIndex = Utils::RandomRange(1, 2);
	std::string filePath = "sound/Attack/sndSwing" + std::to_string(filePathIndex) + ".wav";
	
	SOUND_MGR.PlaySfx(filePath);
}

void Player::AttackBat()
{
	animatorBody.PlayP("animations/Player/Attack/pAttackBat.json");
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
	int filePathIndex = Utils::RandomRange(1, 2);
	std::string filePath = "sound/Attack/sndSwing" + std::to_string(filePathIndex) + ".wav";

	SOUND_MGR.PlaySfx(filePath);
}

void Player::AttackKnife()
{
	animatorBody.PlayP("animations/Player/Attack/pAttackKnife.json");
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
	int filePathIndex = Utils::RandomRange(1, 2);
	std::string filePath = "sound/Attack/sndSwing" + std::to_string(filePathIndex) + ".wav";

	SOUND_MGR.PlaySfx(filePath);
}

void Player::AttackMachinegun()
{
	if (weaponStatus.remainingBullet > 0)
	{
		sceneGame->SpawnBullet()->Fire(Utils::AngleSpread(look, 10), this, weaponStatus);
		for (auto enemy : enemies)
		{
			if (enemy->GetGlobalBounds().intersects(noiseCircle.getGlobalBounds()) && !enemy->isDie() && !enemy->isStun() && !enemy->isStunOnWall())
			{
				if (Utils::RandomRange(0, 1))
					enemy->SetStatus(Enemy::Status::Aggro);
			}
		}
		weaponStatus.remainingBullet--;
		animatorBody.PlayP("animations/Player/Attack/pAttackMachinegun.json");
		SOUND_MGR.PlaySfx("sound/Attack/sndEM16.wav");
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
			if (enemy->GetGlobalBounds().intersects(noiseCircle.getGlobalBounds()) && !enemy->isDie() && !enemy->isStun() && !enemy->isStunOnWall())
			{
				if (Utils::RandomRange(0, 1))
					enemy->SetStatus(Enemy::Status::Aggro);
			}
		}
		weaponStatus.remainingBullet--;
		animatorBody.PlayP("animations/Player/Attack/pAttackShotgun.json");
		SOUND_MGR.PlaySfx("sound/Attack/sndShotgun.wav");
		isAttacking = true;
	}
	else
	{
		// ���л���?
	}
}
