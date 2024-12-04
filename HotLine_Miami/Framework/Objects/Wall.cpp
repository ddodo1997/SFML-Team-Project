#include "stdafx.h"
#include "Wall.h"
#include "WallTable.h"
#include "HitBox.h"
#include "SceneGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss1.h"
#include "Cleaver.h"
Wall::Wall(const std::string& name)
    : GameObject(name)
{
}

void Wall::SetOrigin(Origins preset)
{
    originPreset = preset;
    if (originPreset != Origins::Custom)
    {
        sf::FloatRect rect = GetGlobalBounds();
        origin.x = rect.width * ((int)preset % 3) * 0.5f;
        origin.y = rect.height * ((int)preset / 3) * 0.5f;
    }
    this->setOrigin({ origin.x, origin.y });

}

void Wall::Init()
{
    sortingLayer = SortingLayers::Foreground;
    sortingOrder = 2;

    va.setPrimitiveType(sf::Quads);
}

void Wall::Release()
{
}

void Wall::Reset()
{
    sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
    player = sceneGame->GetPlayer();
    enemies = sceneGame->GetEnemies();
    boss1 = sceneGame->GetBoss1();
    cleaver = sceneGame->GetCleaver();
    setScale(1.f, 1.f);
}

void Wall::Update(float dt)
{
    hitBox.UpdateTr(*this, GetGlobalBounds());
}

void Wall::FixedUpdate(float dt)
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

    if (boss1 == nullptr)
        return;

    auto& boss1HitBox = boss1->GetHitBox();
    if (boss1HitBox.rect.getGlobalBounds().intersects(hitBox.rect.getGlobalBounds()))
    {
        auto& boss1Dir = boss1->GetDirection();
        auto boss1Pos = boss1->GetPosition();
        auto boss1HitBoxHalf = boss1HitBox.rect.getLocalBounds().width * 0.5f;

        if (hitBox.points.top < boss1HitBox.points.center.y && hitBox.points.bottom > boss1HitBox.points.center.y)
        {
            if (boss1HitBox.points.left < hitBox.points.right && boss1HitBox.points.left > hitBox.points.center.x)
            {
                //벽 우측 충돌
                boss1->SetPosition({ hitBox.points.right + boss1HitBoxHalf, boss1Pos.y + boss1Dir.y * 0.5f });
            }
            else if (boss1HitBox.points.right > hitBox.points.left && boss1HitBox.points.right < hitBox.points.center.x)
            {
                //벽 좌측 충돌
                boss1->SetPosition({ hitBox.points.left - boss1HitBoxHalf, boss1Pos.y + boss1Dir.y * 0.5f });
            }
        }
        else
        {
            if (boss1HitBox.points.bottom > hitBox.points.top && boss1HitBox.points.bottom < hitBox.points.center.y)
            {
                //벽 상부 충돌
                boss1->SetPosition({ boss1Pos.x + boss1Dir.x * 0.5f, hitBox.points.top - boss1HitBoxHalf });
            }
            else if (boss1HitBox.points.top < hitBox.points.bottom && boss1HitBox.points.top > hitBox.points.center.y)
            {
                //벽 하부 충돌
                boss1->SetPosition({ boss1Pos.x + boss1Dir.x * 0.5f, hitBox.points.bottom + boss1HitBoxHalf });
            }
        }
    }

    if (cleaver == nullptr)
        return;

    if (cleaver->GetGlobalBounds().intersects(hitBox.rect.getGlobalBounds()))
    {
        cleaver->OnStuckWall();
    }
}

void Wall::Draw(sf::RenderWindow& window)
{
    sf::RenderStates states;
    states.transform = getTransform();
    for (int i = 0; i < va.getVertexCount(); i += 4)
    {
        const sf::Texture* currentTexture = textures[(i / 4) % textures.size()];
        states.texture = currentTexture;
        window.draw(&va[i], 4, sf::Quads, states);
    }

    // 히트박스 그리기
    hitBox.Draw(window);
}

void Wall::SetTexture(const std::string& id)
{
    // 텍스처 설정 함수는 필요 시 구현
}

void Wall::DrawWall(const DataWall& dataWall)
{
    data = dataWall;

    // `sf::Transformable`을 활용하여 위치를 설정합니다.
    setPosition(data.start.x * STAGE_TABLE->GetTileSize().x, data.start.y * STAGE_TABLE->GetTileSize().y);

    textures.clear();
    for (const auto& textureId : data.textureIds)
    {
        textures.push_back(&TEXTURE_MGR.Get(WALL_TABLE->GetFilePath() + textureId));
    }

    if (textures.empty()) return;

    // 벽의 길이 계산 및 오리진 설정
    if (data.start.x == data.end.x)
    {
        length = abs(data.end.y - data.start.y) * STAGE_TABLE->GetTileSize().y;
        SetOrigin(Origins::TC);
    }
    else
    {
        length = abs(data.end.x - data.start.x) * STAGE_TABLE->GetTileSize().x;
        SetOrigin(Origins::ML);
    }

    sf::Vector2f texSize(textures[0]->getSize().x, textures[0]->getSize().y);
    int quadCount = (data.start.x == data.end.x) ? static_cast<int>(length / texSize.y) : static_cast<int>(length / texSize.x);

    va.clear();
    va.resize(quadCount * 4);

    for (int i = 0; i < quadCount; ++i)
    {
        sf::Vector2f offset = (data.start.x == data.end.x) ? sf::Vector2f(0.f, i * texSize.y) : sf::Vector2f(i * texSize.x, 0.f);

        int vertexIndex = i * 4;
        va[vertexIndex].position = offset;
        va[vertexIndex + 1].position = offset + sf::Vector2f(texSize.x, 0.f);
        va[vertexIndex + 2].position = offset + sf::Vector2f(texSize.x, texSize.y);
        va[vertexIndex + 3].position = offset + sf::Vector2f(0.f, texSize.y);

        const sf::Texture* currentTexture = textures[i % textures.size()];
        sf::Vector2f currentTexSize(currentTexture->getSize().x, currentTexture->getSize().y);

        va[vertexIndex].texCoords = { 0.f, 0.f };
        va[vertexIndex + 1].texCoords = { currentTexSize.x, 0.f };
        va[vertexIndex + 2].texCoords = { currentTexSize.x, currentTexSize.y };
        va[vertexIndex + 3].texCoords = { 0.f, currentTexSize.y };
    }
}

sf::FloatRect Wall::GetLocalBounds() const
{
    float width = (data.start.x == data.end.x) ? textures[0]->getSize().x : length;
    float height = (data.start.x == data.end.x) ? length : textures[0]->getSize().y;

    return { 0.f, 0.f, width, height };
}

sf::FloatRect Wall::GetGlobalBounds() const
{
    sf::FloatRect localBounds = GetLocalBounds();
    return getTransform().transformRect(localBounds); 
}
