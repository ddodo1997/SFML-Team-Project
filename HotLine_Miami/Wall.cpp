#include "stdafx.h"
#include "Wall.h"
#include "WallTable.h"
#include "HitBox.h"

Wall::Wall(const std::string& name)
    : GameObject(name)
{
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
    setScale(1.f, 1.f);
}

void Wall::Update(float dt)
{
    hitBox.UpdateTr(*this, GetGlobalBounds());
}

void Wall::Draw(sf::RenderWindow& window)
{
    // 적용된 변환 정보와 텍스처로 벽을 그립니다.
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
