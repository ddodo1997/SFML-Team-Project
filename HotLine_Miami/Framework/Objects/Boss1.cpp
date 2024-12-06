#include "stdafx.h"
#include "Boss1.h"
#include "SceneGame.h"
#include "Player.h"
#include "Cleaver.h"
Boss1::Boss1(const std::string& name)
	: GameObject(name)
{
}

void Boss1::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	legs.setPosition(position);
	collisionBox.setPosition(position);
	meleeHitBox.setPosition(position);
}

void Boss1::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
	legs.setRotation(rotation);
	meleeHitBox.setRotation(rotation);
}

void Boss1::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
	legs.setScale(scale);
}

void Boss1::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
		Utils::SetOrigin(legs, originPreset);
		Utils::SetOrigin(collisionBox, originPreset);
	}
}

void Boss1::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
	legs.setOrigin(origin);
	collisionBox.setOrigin(origin);
}

void Boss1::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Boss1::Release()
{
}

void Boss1::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = sceneGame->GetPlayer();
	cleaver = sceneGame->GetCleaver();
	animatorBody.SetTarget(&body);
	animatorLegs.SetTarget(&legs);
	animatorLegs.Play("animations/Boss1/boss1_legs.json");
	collisionBox.setSize({ 10.f, 10.f });
	meleeHitBox.setFillColor(sf::Color::Transparent);
	meleeHitBox.setOutlineColor(sf::Color::Green);
	meleeHitBox.setOutlineThickness(1.f);
	meleeHitBox.setSize({ 20.f,20.f });
	Utils::SetOrigin(meleeHitBox, Origins::ML);
	SetPosition(sf::Vector2f(100.f, 100.f));
	SetPatterns();
	SetWeaponStatus();
	ChangePattern(Patterns::None);
}

void Boss1::SetPatterns()
{
	pattern.attackTimer = 0.f;
	pattern.attackingTimer = 0.f;
	pattern.crawlTimer = 0.f;
	pattern.crawlingTimer = 0.f;
	pattern.patternCnt = 0;
	pattern.patternTimer = 0.f;
	pattern.prepareAttackTimer = 0.f;
	pattern.pryTimer = 0.f;
	pattern.stunTimer = 0.f;

	pattern.isCrawling = false;
	pattern.isAttacking = false;
	pattern.isOnAttack = false;
	isPhase2 = false;
	isAlive = true;
}

void Boss1::SetWeaponStatus()
{
	auto type = Weapon::WeaponType::Knife;
	weaponStatus = WEAPON_TABLE->Get(type);
}

void Boss1::Update(float dt)
{
	if (player == nullptr)
		return;
	if (InputMgr::GetKeyDown(sf::Keyboard::M))
		OnDie();

	animatorBody.Update(dt);
	if (isWalking)
		animatorLegs.Update(dt);
	if (player->IsDead() || currentPattern == Patterns::Pounded || IsDead())
	{
		
		return;
	}
	hitBox.UpdateTr(collisionBox, collisionBox.getLocalBounds());

	direction = Utils::GetNormal(player->GetPosition() - position);

	if (Utils::Distance(position, player->GetPosition()) > 80.f)
	{
		speed = isPhase2 ? 140.f : 120.f;
	}
	else
	{
		speed = isPhase2 ? -140.f : -120.f;
	}
	if (!IsExcutable())
	{
		pattern.attackTimer += dt;
		if ((Utils::Distance(position, player->GetPosition()) < meleeHitBox.getLocalBounds().width) && pattern.attackDelay <= pattern.attackTimer)
		{
			pattern.attackTimer = 0.f;
			Attack();
		}

		if (!pattern.isAttacking)
		{
			pattern.patternTimer += dt;
			if (pattern.patternTimer >= pattern.patternDelay)
			{
				pattern.patternTimer = 0.f;
				pattern.isAttacking = true;
				pattern.patternCnt < 2 ? ChangePattern(Patterns::Pattern1) : ChangePattern(Patterns::Pattern2);
			}
		}
	}
	switch (currentPattern)
	{
	case Patterns::Pattern1:
		Pattern1(dt);
		break;
	case Patterns::Pattern2:
		Pattern2(dt);
		break;
	case Patterns::PickUpCleaver:
		OnPry(dt);
		break;
	case Patterns::Stun:
		OnStun(dt);
		break;
	case Patterns::Crawl:
		OnCrawl(dt);
		break;
	default:
		SetRotation(Utils::Angle(direction));
		SetPosition(position + direction * speed * dt);
		break;
	}
}

void Boss1::Pattern1(float dt)
{
	if (pattern.isOnAttack)
	{
		pattern.attackingTimer += dt;
		SetPosition(position + pattern.targetDirection * pattern.speed * dt);
		if (pattern.attackingDelay <= pattern.attackingTimer)
		{
			pattern.attackingTimer = 0.f;
			pattern.isOnAttack = false;
			pattern.isAttacking = false;
			pattern.patternCnt++;
			ChangePattern(Patterns::None);
		}
		if (meleeHitBox.getGlobalBounds().intersects(player->GetCollisionBox().getGlobalBounds()))
			player->OnHit(weaponStatus, direction);
	}
	else
	{
		pattern.prepareAttackTimer += dt;
		if (pattern.prepareAttackDelay <= pattern.prepareAttackTimer)
		{
			pattern.prepareAttackTimer = 0.f;
			pattern.isOnAttack = true;
			animatorBody.Play("animations/Boss1/boss1_attack.json");
			animatorBody.PlayQueue("animations/Boss1/boss1_walk.json");
			SetOrigin(Origins::MC);
			SOUND_MGR.PlaySfx("sound/Attack/sndSwing1.wav");
		}
	}
}

void Boss1::Pattern2(float dt)
{
	if (pattern.isOnAttack)
	{
		if (cleaver->isStuckWall())
		{
			pattern.targetDirection = Utils::GetNormal(cleaver->GetPosition() - position);
			SetPosition(position + pattern.targetDirection * pattern.speed * dt);
			if (collisionBox.getGlobalBounds().intersects(cleaver->GetGlobalBounds()))
			{
				cleaver->OnPickUp();
			}
		}
		if (!cleaver->IsActive())
		{
			pattern.isOnAttack = false;
			pattern.isAttacking = false;
			pattern.patternCnt = 0;
			ChangePattern(Patterns::PickUpCleaver);
		}
	}
	else
	{
		pattern.prepareAttackTimer += dt;
		if (pattern.prepareAttackDelay <= pattern.prepareAttackTimer)
		{
			pattern.prepareAttackTimer = 0.f;
			pattern.isOnAttack = true;
			cleaver->Throw(pattern.targetDirection, position);
			animatorBody.Play("animations/Boss1/boss1_throw.json");
			animatorBody.PlayQueue("animations/Boss1/boss1_walk.json");
			SetOrigin(Origins::MC);
		}
	}

}

void Boss1::OnPry(float dt)
{
	pattern.pryTimer += dt;
	if (pattern.pryDelay <= pattern.pryTimer)
	{
		pattern.pryTimer = 0.f;
		ChangePattern(Patterns::None);
	}
}

void Boss1::OnStun(float dt)
{
	pattern.stunTimer += dt;
	if (pattern.stunDelay <= pattern.stunTimer)
	{
		pattern.stunTimer = 0.f;
		animatorBody.Play("animations/Boss1/boss1_getup.json");
		animatorBody.PlayQueue("animations/Boss1/boss1_walk.json");
		SetOrigin(Origins::MC);
		isPhase2 = true;
		ChangePattern(Patterns::None);
	}
}

void Boss1::OnCrawl(float dt)
{
	if (pattern.isCrawling)
	{
		pattern.crawlingTimer += dt;
		if (pattern.crawlingDelay <= pattern.crawlingTimer)
		{
			pattern.crawlingTimer = 0.f;
			pattern.isCrawling = false;
			animatorBody.Stop();
		}
		pattern.targetDirection = Utils::GetNormal(player->GetPosition() - position);
		SetPosition(position + (-pattern.targetDirection) * pattern.crawlingSpeed * dt);
		SetRotation(Utils::Angle(-pattern.targetDirection) + 90.f);
	}
	else
	{
		pattern.crawlTimer += dt;
		if (pattern.crawlDelay <= pattern.crawlTimer)
		{
			pattern.crawlTimer = 0.f;
			pattern.isCrawling = true;
			animatorBody.RePlay();
		}
	}
}

void Boss1::ChangePattern(Patterns pattern)
{
	Patterns prevPattern = currentPattern;
	currentPattern = pattern;
	isWalking = true;
	switch (currentPattern)
	{
	case Patterns::None:
		if (prevPattern == Patterns::PickUpCleaver)
			break;
		animatorBody.Play("animations/Boss1/boss1_walk.json");
		break;
	case Patterns::Pattern1:
	case Patterns::Pattern2:
		animatorBody.Stop();
		SOUND_MGR.PlaySfx("sound/Enemy/sndDrawKnife.wav");
		this->pattern.targetDirection = Utils::GetNormal(player->GetPosition() - position);
		SetRotation(Utils::Angle(this->pattern.targetDirection));
		break;
	case Patterns::PickUpCleaver:
		animatorBody.Play("animations/Boss1/boss1_pry.json");
		animatorBody.PlayQueue("animations/Boss1/boss1_walk.json");
		isWalking = false;
		break;
	case Patterns::Stun:
		animatorBody.Play("animations/Boss1/boss1_stun.json");
		isWalking = false;
		break;
	case Patterns::Crawl:
		animatorBody.Play("animations/Boss1/boss1_start_crawl.json");
		animatorBody.PlayQueue("animations/Boss1/boss1_crawl.json");
		isWalking = false;
		break;
	case Patterns::Pounded:
		speed = 0.f;
		animatorBody.Stop();
		isWalking = false;
		this->pattern.targetDirection = Utils::GetNormal(player->GetPosition() - position);
		SetRotation(Utils::Angle(-this->pattern.targetDirection));
		break;
	case Patterns::Die:
		OnDie();
		break;
	}
	SetOrigin(Origins::MC);
}

void Boss1::Draw(sf::RenderWindow& window)
{
	if (isWalking)
		window.draw(legs);
	window.draw(body);
	if (Variables::isDrawHitBox)
	{
		window.draw(collisionBox);
		window.draw(meleeHitBox);
	}
}

void Boss1::Attack()
{
	if (!IsCanAttack())
		return;

	animatorBody.Play("animations/Boss1/boss1_attack.json");
	animatorBody.PlayQueue("animations/Boss1/boss1_walk.json");
	SetOrigin(Origins::MC);
	if (meleeHitBox.getGlobalBounds().intersects(player->GetCollisionBox().getGlobalBounds()))
		player->OnHit(weaponStatus, direction);
}

void Boss1::OnHit(sf::Vector2f dir)
{
	if (currentPattern != Patterns::PickUpCleaver)
		return;

	!isPhase2 ? ChangePattern(Patterns::Stun) : ChangePattern(Patterns::Crawl);
}

void Boss1::OnDie()
{
	isWalking = false;
	isAlive = false;
	animatorBody.Play("animations/Boss1/boss1_head_boom.json");
	SetOrigin(Origins::MC);
	SetRotation(Utils::Angle(-pattern.targetDirection));
	//anima
}
