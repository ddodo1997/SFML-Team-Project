#include "stdafx.h"
#include "Decoration.h"
#include "SceneGame.h"
#include "Player.h"
#include "Enemy.h"
Decoration::Decoration(const std::string& name)
	: GameObject(name)
{
}

void Decoration::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Decoration::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Decoration::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Decoration::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Decoration::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Decoration::SetTexture(const std::string& textureId)
{
	this->textureId = textureId;
	body.setTexture(TEXTURE_MGR.Get(this->textureId));
}

void Decoration::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Decoration::Release()
{
}

void Decoration::Reset()
{
    sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
    player = sceneGame->GetPlayer();
    enemies = sceneGame->GetEnemies();

	SetOrigin(Origins::MC);
	SetScale({ 1.f, 1.f });
	hitBox.UpdateTr(body, body.getLocalBounds());
}

void Decoration::Update(float dt)
{

}

void Decoration::FixedUpdate(float dt)
{
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

    for (auto enemy : enemies)
    {
        auto& enemyHitBox = enemy->GetHitBox();
        if (enemyHitBox.rect.getGlobalBounds().intersects(hitBox.rect.getGlobalBounds()))
        {
            if (enemy->GetStatus() == Enemy::Status::Stun)
                enemy->SetStatus(Enemy::Status::StunOnWall);

            auto& enemyDir = enemy->GetDirection();
            auto enemyPos = enemy->GetPosition();
            auto enemyHitBoxHalf = enemyHitBox.rect.getLocalBounds().width * 0.5f;

            if (hitBox.points.top < enemyHitBox.points.center.y && hitBox.points.bottom > enemyHitBox.points.center.y)
            {
                if (enemyHitBox.points.left < hitBox.points.right && enemyHitBox.points.left > hitBox.points.center.x)
                {
                    //벽 우측 충돌
                    enemy->SetPosition({ hitBox.points.right + enemyHitBoxHalf, enemyPos.y + enemyDir.y * 0.5f });
                }
                else if (enemyHitBox.points.right > hitBox.points.left && enemyHitBox.points.right < hitBox.points.center.x)
                {
                    //벽 좌측 충돌
                    enemy->SetPosition({ hitBox.points.left - enemyHitBoxHalf, enemyPos.y + enemyDir.y * 0.5f });
                }
            }
            else
            {
                if (enemyHitBox.points.bottom > hitBox.points.top && enemyHitBox.points.bottom < hitBox.points.center.y)
                {
                    //벽 상부 충돌
                    enemy->SetPosition({ enemyPos.x + enemyDir.x * 0.5f, hitBox.points.top - enemyHitBoxHalf });
                }
                else if (enemyHitBox.points.top < hitBox.points.bottom && enemyHitBox.points.top > hitBox.points.center.y)
                {
                    //벽 하부 충돌
                    enemy->SetPosition({ enemyPos.x + enemyDir.x * 0.5f, hitBox.points.bottom + enemyHitBoxHalf });
                }
            }
        }
    }
}

void Decoration::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}
