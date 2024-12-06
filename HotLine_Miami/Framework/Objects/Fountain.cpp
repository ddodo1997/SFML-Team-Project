#include "stdafx.h"
#include "Fountain.h"
#include "Player.h"
#include "Panther.h"
#include "BodyGuard.h"
Fountain::Fountain(const std::string& name)
	: GameObject(name)
{
}

void Fountain::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Fountain::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Fountain::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Fountain::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Fountain::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Fountain::SetPlayer(Player* player)
{
	this->player = player;
}

void Fountain::SetBodyGuard(BodyGuard* body)
{
	bodyguard = body;
}

void Fountain::SetPanthers(Panther* pan1, Panther* pan2)
{
    panther1 = pan1;
    panther2 = pan2;
}

void Fountain::Init()
{
}

void Fountain::Release()
{
}

void Fountain::Reset()
{
	animator.SetTarget(&body);
	animator.Play("animations/Boss2/boss2_fountain.json");
	SetOrigin(Origins::MC);
}

void Fountain::Update(float dt)
{
	animator.Update(dt);
	hitBox.UpdateTr(body, body.getGlobalBounds());


    auto& playerHitBox = player->GetHitBox();
    if (playerHitBox.rect.getGlobalBounds().intersects(hitBox.rect.getGlobalBounds()))
    {
        auto& playerDir = player->GetDirection();
        auto playerPos = player->GetPosition();
        auto playerHitBoxHalf = playerHitBox.rect.getLocalBounds().width * 0.5f;

        if (hitBox.points.top < playerHitBox.points.center.y && hitBox.points.bottom > playerHitBox.points.center.y)
        {
            if (playerHitBox.points.left < hitBox.points.right && playerHitBox.points.left > hitBox.points.center.x)
            {
                //벽 우측 충돌
                player->SetPosition({ hitBox.points.right + playerHitBoxHalf, playerPos.y + playerDir.y * 0.5f });
            }
            else if (playerHitBox.points.right > hitBox.points.left && playerHitBox.points.right < hitBox.points.center.x)
            {
                //벽 좌측 충돌
                player->SetPosition({ hitBox.points.left - playerHitBoxHalf, playerPos.y + playerDir.y * 0.5f });
            }
        }
        else
        {
            if (playerHitBox.points.bottom > hitBox.points.top && playerHitBox.points.bottom < hitBox.points.center.y)
            {
                //벽 상부 충돌
                player->SetPosition({ playerPos.x + playerDir.x * 0.5f, hitBox.points.top - playerHitBoxHalf });
            }
            else if (playerHitBox.points.top < hitBox.points.bottom && playerHitBox.points.top > hitBox.points.center.y)
            {
                //벽 하부 충돌
                player->SetPosition({ playerPos.x + playerDir.x * 0.5f, hitBox.points.bottom + playerHitBoxHalf });
            }
        }
    }


    auto& panther1HitBox = panther1->GetHitBox();
    if (panther1HitBox.rect.getGlobalBounds().intersects(hitBox.rect.getGlobalBounds()))
    {
        auto& panther1Dir = panther1->GetDirection();
        auto panther1Pos = panther1->GetPosition();
        auto panther1HitBoxHalf = panther1HitBox.rect.getLocalBounds().width * 0.5f;

        if (hitBox.points.top < panther1HitBox.points.center.y && hitBox.points.bottom > panther1HitBox.points.center.y)
        {
            if (panther1HitBox.points.left < hitBox.points.right && panther1HitBox.points.left > hitBox.points.center.x)
            {
                //벽 우측 충돌
                panther1->SetPosition({ hitBox.points.right + panther1HitBoxHalf, panther1Pos.y + panther1Dir.y * 0.5f });
            }
            else if (panther1HitBox.points.right > hitBox.points.left && panther1HitBox.points.right < hitBox.points.center.x)
            {
                //벽 좌측 충돌
                panther1->SetPosition({ hitBox.points.left - panther1HitBoxHalf, panther1Pos.y + panther1Dir.y * 0.5f });
            }
        }
        else
        {
            if (panther1HitBox.points.bottom > hitBox.points.top && panther1HitBox.points.bottom < hitBox.points.center.y)
            {
                //벽 상부 충돌
                panther1->SetPosition({ panther1Pos.x + panther1Dir.x * 0.5f, hitBox.points.top - panther1HitBoxHalf });
            }
            else if (panther1HitBox.points.top < hitBox.points.bottom && panther1HitBox.points.top > hitBox.points.center.y)
            {
                //벽 하부 충돌
                panther1->SetPosition({ panther1Pos.x + panther1Dir.x * 0.5f, hitBox.points.bottom + panther1HitBoxHalf });
            }
        }
    }


    auto& bodyguardHitBox = bodyguard->GetHitBox();
    if (bodyguardHitBox.rect.getGlobalBounds().intersects(hitBox.rect.getGlobalBounds()))
    {
        auto& bodyguardDir = bodyguard->GetDirection();
        auto bodyguardPos = bodyguard->GetPosition();
        auto bodyguardHitBoxHalf = bodyguardHitBox.rect.getLocalBounds().width * 0.5f;

        if (hitBox.points.top < bodyguardHitBox.points.center.y && hitBox.points.bottom > bodyguardHitBox.points.center.y)
        {
            if (bodyguardHitBox.points.left < hitBox.points.right && bodyguardHitBox.points.left > hitBox.points.center.x)
            {
                //벽 우측 충돌
                bodyguard->SetPosition({ hitBox.points.right + bodyguardHitBoxHalf, bodyguardPos.y + bodyguardDir.y * 0.5f });
            }
            else if (bodyguardHitBox.points.right > hitBox.points.left && bodyguardHitBox.points.right < hitBox.points.center.x)
            {
                //벽 좌측 충돌
                bodyguard->SetPosition({ hitBox.points.left - bodyguardHitBoxHalf, bodyguardPos.y + bodyguardDir.y * 0.5f });
            }
        }
        else
        {
            if (bodyguardHitBox.points.bottom > hitBox.points.top && bodyguardHitBox.points.bottom < hitBox.points.center.y)
            {
                //벽 상부 충돌
                bodyguard->SetPosition({ bodyguardPos.x + bodyguardDir.x * 0.5f, hitBox.points.top - bodyguardHitBoxHalf });
            }
            else if (bodyguardHitBox.points.top < hitBox.points.bottom && bodyguardHitBox.points.top > hitBox.points.center.y)
            {
                //벽 하부 충돌
                bodyguard->SetPosition({ bodyguardPos.x + bodyguardDir.x * 0.5f, hitBox.points.bottom + bodyguardHitBoxHalf });
            }
        }
    }


    auto& panther2HitBox = panther2->GetHitBox();
    if (panther2HitBox.rect.getGlobalBounds().intersects(hitBox.rect.getGlobalBounds()))
    {
        auto& panther2Dir = panther2->GetDirection();
        auto panther2Pos = panther2->GetPosition();
        auto panther2HitBoxHalf = panther2HitBox.rect.getLocalBounds().width * 0.5f;

        if (hitBox.points.top < panther2HitBox.points.center.y && hitBox.points.bottom > panther2HitBox.points.center.y)
        {
            if (panther2HitBox.points.left < hitBox.points.right && panther2HitBox.points.left > hitBox.points.center.x)
            {
                //벽 우측 충돌
                panther2->SetPosition({ hitBox.points.right + panther2HitBoxHalf, panther2Pos.y + panther2Dir.y * 0.5f });
            }
            else if (panther2HitBox.points.right > hitBox.points.left && panther2HitBox.points.right < hitBox.points.center.x)
            {
                //벽 좌측 충돌
                panther2->SetPosition({ hitBox.points.left - panther2HitBoxHalf, panther2Pos.y + panther2Dir.y * 0.5f });
            }
        }
        else
        {
            if (panther2HitBox.points.bottom > hitBox.points.top && panther2HitBox.points.bottom < hitBox.points.center.y)
            {
                //벽 상부 충돌
                panther2->SetPosition({ panther2Pos.x + panther2Dir.x * 0.5f, hitBox.points.top - panther2HitBoxHalf });
            }
            else if (panther2HitBox.points.top < hitBox.points.bottom && panther2HitBox.points.top > hitBox.points.center.y)
            {
                //벽 하부 충돌
                panther2->SetPosition({ panther2Pos.x + panther2Dir.x * 0.5f, hitBox.points.bottom + panther2HitBoxHalf });
            }
        }
    }
}

void Fountain::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}
