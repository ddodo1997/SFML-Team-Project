#include "stdafx.h"
#include "PathFinder.h"
#include "SceneGame.h"
#include "Wall.h"

namespace std {
    template <>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& vec) const {
            return (static_cast<std::size_t>(vec.x) << 16) ^ static_cast<std::size_t>(vec.y);
        }
    };
}

PathFinder::PathFinder()
{
    map = STAGE_TABLE->GetFloorTiles();
    mapSize = STAGE_TABLE->GetTileCount();
    tileSize = static_cast<sf::Vector2f>(STAGE_TABLE->GetTileSize());
}

bool PathFinder::IsValid(sf::Vector2i pos)
{
    return (pos.x >= 0 && pos.x < mapSize.x && pos.y >= 0 && pos.y < mapSize.y);
}

bool PathFinder::IsWalkable(sf::Vector2i pos, sf::Vector2i nextPos)
{
    auto walls = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetWalls();
    sf::Vector2f boundaryPos;

    if (pos.x == nextPos.x)
    {
        boundaryPos = { pos.x * tileSize.x, std::min(pos.y, nextPos.y) * tileSize.y + tileSize.y * 0.5f };
    }
    else if(pos.y == nextPos.y)
    {
        boundaryPos = { std::min(pos.x, nextPos.x) * tileSize.x + tileSize.x * 0.5f, pos.y * tileSize.y };
    }

    for (const auto& wall : walls)
    {
        if (wall->GetGlobalBounds().contains(boundaryPos))
        {
            return false;
        }
    }

    return true;
}

std::vector<sf::Vector2i> PathFinder::GetNeighborNodes(sf::Vector2i pos)
{
    std::vector<sf::Vector2i> neighbors;
    std::vector<sf::Vector2i> directions = {
        {0, 1},  
        {1, 0},
        {0, -1}, 
        {-1, 0}
    };

    for (const auto& dir : directions)
    {
        sf::Vector2i neighborPos = pos + dir;
        if (IsValid(neighborPos) && IsWalkable(pos, neighborPos))
        {
            neighbors.push_back(neighborPos);
        }
    }
    return neighbors;
}

std::vector<sf::Vector2f> PathFinder::FindPath(sf::Vector2f startPos, sf::Vector2f targetPos)
{
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openQueue;
    std::unordered_map<sf::Vector2i, Node> allNodes;
    std::list<sf::Vector2i> closedList;

    sf::Vector2i startTileIdx = { static_cast<int>(startPos.x / tileSize.x), static_cast<int>(startPos.y / tileSize.y) };
    sf::Vector2i TargetTileIdx = { static_cast<int>(targetPos.x / tileSize.x), static_cast<int>(targetPos.y / tileSize.y) };

    Node* startNode = &allNodes[startTileIdx];
    startNode->position = startTileIdx;
    startNode->gCost = 0;
    startNode->hCost = GetHeuristic(startTileIdx, TargetTileIdx);
    openQueue.push(startNode);

    while (!openQueue.empty())
    {
        Node* currentNode = openQueue.top();
        openQueue.pop();

        currentNode->isClosed = true;
        if (currentNode->position == TargetTileIdx)
        {
            std::vector<sf::Vector2f> path;
            Node* pathNode = currentNode;

            while (pathNode)
            {
                path.push_back({pathNode->position.x * tileSize.x + tileSize.x * 0.5f, pathNode->position.x * tileSize.y + tileSize.y * 0.5f });
                pathNode = pathNode->parent;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

        for (auto& neighborPos : GetNeighborNodes(currentNode->position))
        {
       
            Node* neighborNode = &allNodes[neighborPos];
            if (neighborNode->isClosed)
            {
                continue;
            }

            float nextGCost = currentNode->gCost + 1.0f;
            if (nextGCost < neighborNode->gCost)
            {
                neighborNode->position = neighborPos;
                neighborNode->gCost = nextGCost;
                neighborNode->hCost = GetHeuristic(neighborPos, TargetTileIdx);
                neighborNode->parent = currentNode;

                openQueue.push(neighborNode);
            }
        }
    }
    return std::vector<sf::Vector2f>(); 
}

float PathFinder::GetHeuristic(sf::Vector2i start, sf::Vector2i end)
{
    return std::abs(start.x - end.x) + std::abs(start.y - end.y);
}
